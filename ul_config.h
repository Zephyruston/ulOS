#ifndef UL_CONFIG_H
#define UL_CONFIG_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* ==================== 基础类型定义 ==================== */
typedef float               ul_fp32_t;
typedef double              ul_fp64_t;
typedef int8_t              ul_int8_t;
typedef uint8_t             ul_uint8_t;
typedef int16_t             ul_int16_t;
typedef uint16_t            ul_uint16_t;
typedef int32_t             ul_int32_t;
typedef uint32_t            ul_uint32_t;
typedef size_t              ul_size_t;
typedef unsigned long long  ul_tick_t;
typedef uint32_t            ul_base_t;

typedef bool                ul_bool_t;
#define UL_TRUE             true
#define UL_FALSE            false

/* ==================== 常量定义 ==================== */
#define UL_NULL     (0)
#define UL_ALIGN_SIZE   4
#define UL_OBJECT_NAME_MAX_LENGTH   (24)  // 请4字节对齐

/* ==================== 宏定义 ==================== */
#define ul_inline                   static __inline
#define ul_weak                     __attribute__((weak))
#define UL_ALIGN_DOWN(size,align)   ((size)&~((align)-1))
#define UL_ALIGN_4(size)            ((size)&~((4)-1))

/* ==================== 错误码定义 ==================== */
typedef enum {
    UL_EOK = 0,
    UL_ERROR,
    UL_ETIMEOUT,
    UL_ENULL,
    UL_EFULL,
    UL_EEMPTY,
} ul_ecode;

/* ==================== 系统配置 ==================== */
// 任务优先级
#define ULOS_CONFIG_MAX_PRIORITY    ( 32 )   // 无限制，0为最高优先级
#define ULOS_MAX_TICK               (0xFFFFFFFFFFFFFFFFUL)
#define ULOS_MAX_DELAY              ULOS_MAX_TICK

// 功能开关
#define ULOS_CONFIG_USE_TOPIC          ( 0 )
#define ULOS_CONFIG_USE_MUTEX          ( 1 )
#define ULOS_CONFIG_USE_QUEUE          ( 1 )
#define ULOS_CONFIG_USE_SEMAPHORE      ( 1 )
#define ULOS_CONFIG_USE_EVENT          ( 1 )

#define ULOS_CONFIG_USE_TIMER                      ( 1 )
#define ULOS_CONFIG_TIMER_THREAD_PRIORITY          (ULOS_CONFIG_MAX_PRIORITY - 2)
#define ULOS_CONFIG_TIMER_THREAD_STACK_SIZE        ( 512 )

#define ULOS_CONFIG_USE_IDLEHOOK   ( 0 )
#define ULOS_CONFIG_USE_TICKHOOK   ( 0 )

#define ULOS_CONFIG_SCHED_ALG_FFS   ( 1 )

// 内存配置
#define UL_HEAP_SIZE        ((ul_size_t)(1024 * 10))   /* 堆大小 x KB */

/* ==================== 断言配置 ==================== */
#define ULOS_CONFIG_USE_ASSERT      ( 1 )
#if ( ULOS_CONFIG_USE_ASSERT == 1 )
#define UL_ASSERT(EX)                                                         \
if (!(EX))                                                                    \
{                                                                             \
    ul_assert_handler(#EX, __FILE__, __LINE__);                           \
} 
#else
#define UL_ASSERT(x)   
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* UL_CONFIG_H */
