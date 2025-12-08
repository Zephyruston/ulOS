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
#define ULOS_CONFIG_MAX_PRIORITY    ( 8 )   // 无限制，0为最高优先级
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


// 内存配置
#define UL_HEAP_SIZE        ((ul_size_t)(1024 * 10))   /* 堆大小 x KB */

/* ==================== 断言配置 ==================== */
#define ULOS_CONFIG_USE_ASSERT      ( 0 )
#if ( ULOS_CONFIG_USE_ASSERT == 1 )
#define UL_ASSERT(x)   
#else
#define UL_ASSERT(x)   
#endif
/* ==================== 平台适配层 ==================== */
#ifdef RTTHREAD
#include "rtthread.h"
#define ul_malloc   rt_malloc
#define ul_free     rt_free
#define ul_memcpy   rt_memcpy
#define ul_delay_ms rt_thread_mdelay

#elif defined(FREERTOS)
#include "freertos.h"
#define ul_malloc   pvPortMalloc
#define ul_free     vPortFree
#define ul_memcpy   memcpy
#define ul_delay_ms vTaskDelay

#else
// 默认使用标准库
#include <stdlib.h>
#include <string.h>
#include <main.h>

//#define ul_malloc   malloc
//#define ul_free     free
//#define ul_memcpy   memcpy
//#define ul_delay_ms HAL_Delay
#endif  /* 平台适配层 */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* UL_CONFIG_H */
