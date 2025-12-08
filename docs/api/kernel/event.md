# 事件 (Event) API 文档

需要配置`#define ULOS_CONFIG_USE_EVENT	( 1 )`

## 概述

一种线程间通信机制，允许一个或多个线程等待特定事件的发生。事件使用32位的标志位来表示不同的事件，每个位可以独立设置和清除。

## 数据类型

### ul_event_t

事件控制块结构体，用于管理事件对象。

```c
typedef struct ul_event
{
    ul_ipc_object_t parent;            /* 继承自IPC对象 */
    uint32_t event_set;                /* 当前事件集合 */
} ul_event_t;
```

## 宏

### 事件标志选项

```c
#define UL_EVENT_FLAG_AND   0x01      /* 逻辑与 */
#define UL_EVENT_FLAG_OR    0x02      /* 逻辑或 */
#define UL_EVENT_FLAG_CLEAR 0x04      /* 清除标志 */
```

## API 

### ul_event_create

创建一个事件对象。

```c
ul_event_t* ul_event_create(const char *name);
```

**参数:**
- `name`: 事件名称，用于调试和识别

**返回值:**
- 成功: 返回指向事件控制块的指针
- 失败: 返回NULL

**示例:**
```c
ul_event_t *event = ul_event_create("my_event");
if (event == NULL) {
    // 创建失败处理
}
```

---

### ul_event_delete

删除一个事件对象，释放相关资源。

```c
ul_ecode ul_event_delete(ul_event_t *event);
```

**参数:**
- `event`: 指向要删除的事件控制块的指针

**返回值:**
- `UL_EOK`: 删除成功
- `UL_ENULL`: 参数为空

**示例:**
```c
ul_ecode ret = ul_event_delete(event);
if (ret != UL_EOK) {
    // 删除失败处理
}
```

---

### ul_event_send

发送事件，设置指定的事件标志。

```c
ul_ecode ul_event_send(ul_event_t *event, ul_uint32_t set);
```

**参数:**
- `event`: 指向事件控制块的指针
- `set`: 要设置的事件标志位，可以是多个位的组合

**返回值:**
- `UL_EOK`: 发送成功
- `UL_ENULL`: 参数为空

**示例:**
```c
#define EVENT_FLAG1 (1 << 0)
#define EVENT_FLAG2 (1 << 1)

// 设置单个标志
ul_event_send(event, EVENT_FLAG1);

// 设置多个标志
ul_event_send(event, EVENT_FLAG1 | EVENT_FLAG2);
```

---

### ul_event_recv

接收事件，等待指定的事件标志。

```c
ul_ecode ul_event_recv(ul_event_t *event, 
                      ul_uint32_t set, 
                      ul_uint8_t option, 
                      ul_tick_t timeout, 
                      ul_uint32_t *recved);
```

**参数:**
- `event`: 指向事件控制块的指针
- `set`: 感兴趣的事件标志位
- `option`: 接收选项，可以是以下值的组合:
  - `UL_EVENT_FLAG_AND`: 等待所有指定的事件标志都设置
  - `UL_EVENT_FLAG_OR`: 等待任意一个指定的事件标志被设置
  - `UL_EVENT_FLAG_CLEAR`: 接收成功后清除接收到的事件标志
- `timeout`: 等待超时时间，单位为系统时钟节拍
  - `0`: 非阻塞模式
  - `ULOS_MAX_DELAY`: 永久等待
- `recved`: 指向存储接收到的事件标志的变量的指针，可以为NULL

**返回值:**
- `UL_EOK`: 接收成功
- `UL_ENULL`: 参数为空
- `UL_ETIMEOUT`: 等待超时

**示例:**
```c
#define EVENT_FLAG1 (1 << 0)
#define EVENT_FLAG2 (1 << 1)
ul_uint32_t received_events;

// 等待任意一个标志被设置，接收后清除
ul_ecode ret = ul_event_recv(event, 
                            EVENT_FLAG1 | EVENT_FLAG2, 
                            UL_EVENT_FLAG_OR | UL_EVENT_FLAG_CLEAR, 
                            ULOS_MAX_DELAY, 
                            &received_events);
if (ret == UL_EOK) {
    // 处理接收到的事件
    if (received_events & EVENT_FLAG1) {
        // 处理EVENT_FLAG1
    }
    if (received_events & EVENT_FLAG2) {
        // 处理EVENT_FLAG2
    }
}
```

---

## 使用示例

下面是一个完整的事件使用示例：

```c
#include "ulOS_ipc.h"

// 定义事件标志
#define EVENT_FLAG3 (1 << 3)
#define EVENT_FLAG5 (1 << 5)

// 事件对象
ul_event_t *event;

// 线程1 - 等待事件
void thread1_entry(void *param)
{
    ul_uint32_t received;

    while (1) {
        // 等待EVENT_FLAG3或EVENT_FLAG5，接收后清除标志
        if (ul_event_recv(event, 
                         (EVENT_FLAG3 | EVENT_FLAG5), 
                         UL_EVENT_FLAG_OR | UL_EVENT_FLAG_CLEAR, 
                         ULOS_MAX_DELAY, 
                         &received) == UL_EOK) {
            if (received & EVENT_FLAG3) {
                // 处理EVENT_FLAG3
            }
            else if (received & EVENT_FLAG5) {
                // 处理EVENT_FLAG5
            }
        }
    }
}

// 线程2 - 发送事件
void thread2_entry(void *param)
{
    // 发送EVENT_FLAG3
    ul_event_send(event, EVENT_FLAG3);

    // 延时一段时间
    ul_thread_delay(100);

    // 发送EVENT_FLAG5
    ul_event_send(event, EVENT_FLAG5);

    // 延时一段时间
    ul_thread_delay(100);

    // 同时发送EVENT_FLAG3和EVENT_FLAG5
    ul_event_send(event, EVENT_FLAG3 | EVENT_FLAG5);
    
    return;
}

void main_thread(void *p)
{
    // 创建事件对象
    event = ul_event_create("event");
    if (event == NULL) {
        // 创建失败处理
        return -1;
    }

    // 创建线程
    ul_thread_create("thread1", thread1_entry, NULL, 512, 5, 10);
    ul_thread_create("thread2", thread2_entry, NULL, 512, 5, 10);

    return;
}
```

## 注意事项

1. 事件标志是32位无符号整数，最多可以表示32个不同的事件。
2. 多个线程可以同时等待同一个事件对象。
3. 多个线程可以同时向同一个事件对象发送事件。
4. 使用`UL_EVENT_FLAG_CLEAR`选项时，接收成功后会自动清除接收到的事件标志。
5. 事件对象在使用完毕后应当调用`ul_event_delete`删除，释放系统资源。
