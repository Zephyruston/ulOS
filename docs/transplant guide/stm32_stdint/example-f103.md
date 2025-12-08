1.配置好基础工程后，关闭stm32f10x_it.c文件里中断服务函数代码

![0](D:\MCU\Task\Dasanzy\ulOS-main\ulOS-main\docs\transplant guide\stm32_stdint\pic\0.png)

2.新建文件夹`ulOS`

![1](D:\MCU\Task\Dasanzy\ulOS-main\ulOS-main\docs\transplant guide\stm32_stdint\pic\1.png)

3.将源码中的`kernel`，`platform`文件夹与`ul_config.h`移至刚刚创建的文件夹内

![2](D:\MCU\Task\Dasanzy\ulOS-main\ulOS-main\docs\transplant guide\stm32_stdint\pic\2.png)4.添加头文件路径

![3](D:\MCU\Task\Dasanzy\ulOS-main\ulOS-main\docs\transplant guide\stm32_stdint\pic\3.png)5.添加文件夹，分别添加`ulOS/kernel/src` 与 `ulOS/platform/cortex-m3`内全部文件

![4](D:\MCU\Task\Dasanzy\ulOS-main\ulOS-main\docs\transplant guide\stm32_stdint\pic\4.png)

6.在ul_config.h文件里将#include<main.h>这一段注释掉

![5](D:\MCU\Task\Dasanzy\ulOS-main\ulOS-main\docs\transplant guide\stm32_stdint\pic\5.png)

![6](D:\MCU\Task\Dasanzy\ulOS-main\ulOS-main\docs\transplant guide\stm32_stdint\pic\6.png)



7.编译无错误，main函数内添加操作系统启动代码，创建启动线程与几个测试线程，具体代码见template文件

```c
#include "ulOS_thread.h"

void thread0_entry(void *p)
{
    while(1)
    {
        LED_Turn(1);
        ul_thread_delay(100);
    }
}

void thread1_entry(void *p)
{
    while(1)
    {
        LED_Turn(2);
        ul_thread_delay(500);
    }
}

void thread2_entry(void *p)
{
    while(1)
    {
        LED_Turn(3);
        ul_thread_delay(1000);
    }
}

void start_thread_entry(void *p)
{
    ul_thread_t *tid0 = ul_thread_create("t0", thread0_entry, UL_NULL, 512, 0, 1);
    ul_thread_t *tid1 = ul_thread_create("t1", thread1_entry, UL_NULL, 512, 0, 1);
    ul_thread_t *tid2 = ul_thread_create("t2", thread2_entry, UL_NULL, 512, 0, 1);
    
    ul_thread_startup(tid0);
    ul_thread_startup(tid1);
    ul_thread_startup(tid2);
    
    while(1)
    {
        ul_thread_delay(10);
    }
}



int main(void)
{
    /*模块初始化*/
    LED_Init();
    Timer_Init();
    ul_kernel_init();
    
    ul_thread_startup(ul_thread_create("start",start_thread_entry,UL_NULL,512,1,1));
    
    ul_scheduler_start();
    while (1)
    {
        
    }

}

```

8.自己写一个1ms的中断服务函数，添加代码如下，编译无错误

```c

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
        ul_tick_increase();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

```

![9](D:\MCU\Task\Dasanzy\ulOS-main\ulOS-main\docs\transplant guide\stm32_stdint\pic\9.png)

9. 编译无错误，下载发现三个LED交替闪烁