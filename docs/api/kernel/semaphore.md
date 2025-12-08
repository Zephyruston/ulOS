# 信号量 (Semaphore) API 文档

需要配置`#define ULOS_CONFIG_USE_SEMAPHORE	( 1 )`

## 概述

信号量是一种用于线程同步和资源管理的IPC机制，通过维护一个计数器来控制对共享资源的访问。信号量可用于限制同时访问某个资源的线程数量，或者用于线程间的同步通知。

## 数据类型

### ul_sem_t

信号量控制块结构体，用于管理信号量对象。

```c
typedef struct ul_semaphore
{
    ul_ipc_object_t parent;            /* 继承自IPC对象 */
    ul_uint16_t count;                 /* 信号量计数值 */
} ul_sem_t;
```

## API 

### ul_sem_create

创建一个信号量对象。

```c
ul_sem_t* ul_sem_create(const char *name,
                        ul_uint16_t init_count);
```

**参数:**
- `name`: 信号量对象名字
- `init_count`: 信号量初始计数值

**返回值:**

- 成功: 返回指向信号量控制块的指针
- 失败: 返回NULL

**示例:**
```c
ul_sem_t *sem = ul_sem_create("sem", 1);
if (sem == NULL) {
    // 创建失败处理
}
```

---

### ul_sem_delete

删除一个信号量对象，释放相关资源。

```c
ul_ecode ul_sem_delete(ul_sem_t *sem);
```

**参数:**
- `sem`: 指向要删除的信号量控制块的指针

**返回值:**
- `UL_EOK`: 删除成功
- `UL_ENULL`: 参数为空

**示例:**
```c
ul_ecode ret = ul_sem_delete(sem);
if (ret != UL_EOK) {
    // 删除失败处理
}
```

---

### ul_sem_take

获取信号量，如果信号量计数值为0，则线程将被阻塞直到信号量可用或超时。

```c
ul_ecode ul_sem_take(ul_sem_t *sem, ul_tick_t timeout);
```

**参数:**
- `sem`: 指向信号量控制块的指针
- `timeout`: 等待超时时间，单位为系统时钟节拍
  - `0`: 非阻塞模式
  - `ULOS_MAX_DELAY`: 永久等待

**返回值:**
- `UL_EOK`: 获取成功
- `UL_ENULL`: 参数为空
- `UL_ETIMEOUT`: 等待超时

**示例:**
```c
// 永久等待获取信号量
ul_ecode ret = ul_sem_take(sem, ULOS_MAX_DELAY);
if (ret == UL_EOK) {
    // 获取成功，访问共享资源
    // ...

    // 释放信号量
    ul_sem_post(sem);
}
```

---

### ul_sem_give

释放信号量，增加信号量的计数值，并唤醒等待该信号量的线程。

```c
ul_ecode ul_sem_give(ul_sem_t *sem);
```

**参数:**

- `sem`: 指向信号量控制块的指针

**返回值:**
- `UL_EOK`: 释放成功
- `UL_ENULL`: 参数为空
- `UL_EFULL`: 信号量已达到最大值

**示例:**
```c
// 释放信号量
ul_ecode ret = ul_sem_give(sem);
if (ret != UL_EOK) {
    // 释放失败处理
}
```

---

## 使用示例

下面是一个完整的信号量使用示例，演示了生产者-消费者模型：

```c
#include "ulOS_thread.h"
#include "ulOS_ipc.h"

/* 指向信号量的指针 */
ul_sem_t *dynamic_sem = UL_NULL;

ul_thread_t *sem_thread1;
ul_thread_t *sem_thread2;

static void _thread1_entry(void *parameter)
{
    static ul_uint8_t count = 0;
  
    while(1)
    {
        if(count <= 100)
        {
            count++;           
        }
        else
            return; 
        
        /* count每计数10次，就释放一次信号量 */
         if(0 == (count % 10))
        {
            ul_sem_give(dynamic_sem);   
            ul_enter_critical();
            printf("t1 release a dynamic semaphore.\r\n" ); 
            ul_exit_critical();
        }
    }
}



static void _thread2_entry(void *parameter)
{
    static ul_ecode result;
    static ul_uint8_t number = 0;
    while(1)
    {
        /* 永久方式等待信号量，获取到信号量，则执行number自加的操作 */
        result = ul_sem_take(dynamic_sem, ULOS_MAX_DELAY);
        ul_enter_critical();
        if (result != UL_EOK)
        {        
            printf("t2 take a dynamic semaphore, failed.\r\n");
            ul_sem_delete(dynamic_sem);
            return;
        }
        else
        {      
            number++;             
            printf("t2 take a dynamic semaphore. number = %d\r\n" ,number);                        
        }
        ul_exit_critical();
    }   
}

/* 测试函数 */
void example_semaphore(void)
{
    
    /* 创建一个动态信号量，初始值是0 */
    dynamic_sem = ul_sem_create("dsem", 0);
    if (dynamic_sem == UL_NULL)
    {
        printf("create dynamic semaphore failed.\r\n");
        return;
    }
    else
    {
        printf("create done. dynamic semaphore value = 0.\r\n");
    }

    sem_thread1 = ul_thread_create("sem_t1",
                               _thread1_entry,
                               NULL,
                               1024,
                               1,
                               1);
    ul_thread_startup(sem_thread1);
    
    sem_thread2 = ul_thread_create("sem_t2",
                               _thread2_entry,
                               NULL,
                               1024,
                               0,
                               1);
    ul_thread_startup(sem_thread2);
    
}
```

## 注意事项

1. 信号量对象在使用完毕后应当调用`ul_sem_delete`删除，释放系统资源。
2. 在使用信号量保护共享资源时，通常需要配合互斥锁使用，以防止竞态条件。
