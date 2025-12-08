;/**
; * @addtogroup CORTEX-M4
; */
;/*@{*/

SCB_VTOR        EQU     0xE000ED08               ; Vector Table Offset Register
NVIC_INT_CTRL   EQU     0xE000ED04               ; interrupt control state register
NVIC_SYSPRI2    EQU     0xE000ED20               ; system priority register (2)
NVIC_PENDSV_PRI EQU     0xFFFF0000               ; PendSV and SysTick priority value (lowest)
NVIC_PENDSVSET  EQU     0x10000000               ; value to trigger PendSV exception

    AREA |.text|, CODE, READONLY, ALIGN=2
    THUMB
    REQUIRE8
    PRESERVE8

    IMPORT ul_thread_switch_interrupt_flag
    IMPORT ul_interrupt_from_thread_sp
    IMPORT ul_interrupt_to_thread_sp

;/*
; * ul_base_t ul_hw_interrupt_disable();
; */
ul_hw_interrupt_disable    PROC
    EXPORT  ul_hw_interrupt_disable
    MRS     r0, PRIMASK
    CPSID   I
    BX      LR
    ENDP

;/*
; * void ul_hw_interrupt_enable(ul_base_t level);
; */
ul_hw_interrupt_enable    PROC
    EXPORT  ul_hw_interrupt_enable
    MSR     PRIMASK, r0
    BX      LR
    ENDP

;/*
; * void ul_hw_context_switch(rt_uint32 from, rt_uint32 to);
; * r0 --> from
; * r1 --> to
; */
ul_hw_context_switch_interrupt
    EXPORT ul_hw_context_switch_interrupt
ul_hw_context_switch    PROC
    EXPORT ul_hw_context_switch

    ; 设置任务切换标志
    LDR     r2, =ul_thread_switch_interrupt_flag  ; 加载切换标志地址到r2
    LDR     r3, [r2]                               ; 读取当前标志值到r3
    CMP     r3, #1                                 ; 比较标志是否为1
    BEQ     _reswitch                              ; 如果已经是1，跳转到_reswitch
    MOV     r3, #1                                 ; 将r3设为1
    STR     r3, [r2]                               ; 设置切换标志为1

    ; 设置源线程
    LDR     r2, =ul_interrupt_from_thread_sp          ; 加载源线程指针地址
    STR     r0, [r2]                               ; 存储当前线程指针(r0)到from_thread

_reswitch
    ; 设置目标线程
    LDR     r2, =ul_interrupt_to_thread_sp           ; 加载目标线程指针地址
    STR     r1, [r2]                               ; 存储目标线程指针(r1)到to_thread

    ; 触发PendSV异常（这将导致实际的上下文切换）
    LDR     r0, =NVIC_INT_CTRL                     ; 加载中断控制状态寄存器地址
    LDR     r1, =NVIC_PENDSVSET                    ; 加载触发PendSV的值
    STR     r1, [r0]                               ; 写入以触发PendSV异常

    BX      LR                                     ; 返回
    ENDP


; r0 --> switch from thread stack
; r1 --> switch to thread stack
; psr, pc, lr, r12, r3, r2, r1, r0 are pushed into [from] stack
PendSV_Handler   PROC
    EXPORT PendSV_Handler

    ; 禁用中断以保护上下文切换过程
    MRS     r2, PRIMASK              ; 保存当前中断状态到r2
    CPSID   I                        ; 禁用中断（设置PRIMASK）

    ; 检查是否需要进行任务切换
    LDR     r0, =ul_thread_switch_interrupt_flag  ; 加载切换标志地址
    LDR     r1, [r0]                               ; 读取标志值
    CBZ     r1, pendsv_exit         ; 如果标志为0，直接退出

    ; 清除任务切换标志
    MOV     r1, #0x00               ; 将r1设为0
    STR     r1, [r0]                ; 清除切换标志

    ; 检查是否需要保存当前任务上下文
    LDR     r0, =ul_interrupt_from_thread_sp  ; 获取当前任务指针地址
    LDR     r1, [r0]                        ; 读取当前任务指针
    CBZ     r1, switch_to_thread    ; 如果是第一次切换（from为0），跳过保存

    ; 保存当前任务的上下文
    MRS     r1, psp                 ; 获取当前任务的栈指针(PSP)
    
    IF      {FPU} != "SoftVFP"
    TST     lr, #0x10               ; if(!EXC_RETURN[4])
    VSTMFDEQ  r1!, {d8 - d15}       ; push FPU register s16~s31
    ENDIF
        
    STMFD   r1!, {r4 - r11}         ; 将r4-r11压入栈中
    
    IF      {FPU} != "SoftVFP"
    MOV     r4, #0x00               ; flag = 0
    TST     lr, #0x10               ; if(!EXC_RETURN[4])
    MOVEQ   r4, #0x01               ; flag = 1
    STMFD   r1!, {r4}               ; push flag
    ENDIF
    
    LDR     r0, [r0]                ; 获取当前任务控制块地址
    STR     r1, [r0]                ; 更新任务控制块中的栈指针

switch_to_thread
    ; 准备切换到新任务
    LDR     r1, =ul_interrupt_to_thread_sp  ; 获取目标任务指针地址
    LDR     r1, [r1]                        ; 读取目标任务指针
    LDR     r1, [r1]                        ; 获取目标任务的栈指针

    IF      {FPU} != "SoftVFP"
    LDMFD   r1!, {r3}               ; pop flag
    ENDIF

    ; 恢复目标任务的上下文
    LDMFD   r1!, {r4 - r11}         ; 从栈中弹出r4-r11
    
    IF      {FPU} != "SoftVFP"
    CMP     r3,  #0                 ; if(flag_r3 != 0)
    VLDMFDNE  r1!, {d8 - d15}       ; pop FPU register s16~s31
    ENDIF
    
    MSR     psp, r1                 ; 更新PSP为目标任务的栈指针

    IF      {FPU} != "SoftVFP"
    ORR     lr, lr, #0x10           ; lr |=  (1 << 4), clean FPCA.
    CMP     r3,  #0                 ; if(flag_r3 != 0)
    BICNE   lr, lr, #0x10           ; lr &= ~(1 << 4), set FPCA.
    ENDIF

pendsv_exit
    ; 恢复中断状态
    MSR     PRIMASK, r2             ; 恢复之前保存的中断状态

    ; 设置返回状态
    ORR     lr, lr, #0x04           ; 设置EXC_RETURN，确保返回时使用PSP
    BX      lr                      ; 返回到新任务

    ENDP


;/*
; * void ul_hw_context_switch_to(rt_uint32 to);
; * r0 --> to
; * this fucntion is used to perform the first thread switch
; */
ul_hw_context_switch_to    PROC
    EXPORT ul_hw_context_switch_to
    ; 设置要切换到的目标线程
    LDR     r1, =ul_interrupt_to_thread_sp    ; 加载rt_interrupt_to_thread的地址到r1
    STR     r0, [r1]                       ; 将r0（目标线程栈指针）存入rt_interrupt_to_thread

    IF      {FPU} != "SoftVFP"
    ; CLEAR CONTROL.FPCA
    MRS     r2, CONTROL             ; read
    BIC     r2, #0x04               ; modify
    MSR     CONTROL, r2             ; write-back
    ENDIF
    
    ; 设置源线程为0（因为是第一次切换，没有源线程）
    LDR     r1, =ul_interrupt_from_thread_sp  ; 加载rt_interrupt_from_thread的地址到r1
    MOV     r0, #0x0                       ; 将r0设置为0
    STR     r0, [r1]                       ; 将0存入rt_interrupt_from_thread

    ; 设置线程切换标志为1
    LDR     r1, =ul_thread_switch_interrupt_flag  ; 加载标志地址到r1
    MOV     r0, #1                              ; 设置r0为1
    STR     r0, [r1]                             ; 将标志设置为1

    ; 设置PendSV优先级（设置为最低优先级）
    LDR     r0, =NVIC_SYSPRI2               ; 加载系统优先级寄存器地址
    LDR     r1, =NVIC_PENDSV_PRI           ; 加载PendSV优先级值（最低）
    LDR.W   r2, [r0,#0x00]                 ; 读取当前寄存器值
    ORR     r1,r1,r2                       ; 将PendSV优先级与当前值合并
    STR     r1, [r0]                       ; 写回修改后的值

    ; 触发PendSV异常（这将导致上下文切换）
    LDR     r0, =NVIC_INT_CTRL             ; 加载中断控制状态寄存器地址
    LDR     r1, =NVIC_PENDSVSET            ; 加载触发PendSV的值
    STR     r1, [r0]                       ; 写入以触发PendSV异常

    ; 恢复主栈指针(MSP).因为后续永远不会回来了，之前因为函数调用和局部变量 压的栈也没用了
    LDR     r0, =SCB_VTOR                  ; 加载向量表偏移寄存器地址
    LDR     r0, [r0]                       ; 读取向量表地址
    LDR     r0, [r0]                       ; 读取向量表第一个条目（初始栈指针）
    MSR     msp, r0                        ; 设置MSP

    ; 使能处理器级别的中断
    CPSIE   F                              ; 使能fault异常
    CPSIE   I                              ; 使能IRQ中断

    ; 确保PendSV异常在后续操作之前被处理
    DSB                                     ; 数据同步屏障
    ISB                                     ; 指令同步屏障

    ; 正常情况下不会执行到这里
    ENDP

    ALIGN   4

    END
