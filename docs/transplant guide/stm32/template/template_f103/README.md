# ulOS API参考手册

## 1. 系统配置

### 配置文件
- **文件路径**: `ul_config.h`
- **功能**: 包含ulOS的基础类型定义、常量定义、错误码定义和系统配置选项

### 重要配置选项
```c
// 最大优先级数（0为最高优先级）
#define ULOS_CONFIG_MAX_PRIORITY    ( 8 )

// 功能模块开关
#define ULOS_CONFIG_USE_TOPIC       ( 0 )  // 启用/禁用主题功能
#define ULOS_CONFIG_USE_MUTEX       ( 1 )  // 启用/禁用互斥锁
#define ULOS_CONFIG_USE_QUEUE       ( 1 )  // 启用/禁用消息队列
#define ULOS_CONFIG_USE_SEMAPHORE   ( 1 )  // 启用/禁用信号量
#define ULOS_CONFIG_USE_EVENT       ( 1 )  // 启用/禁用事件
#define ULOS_CONFIG_USE_TIMER       ( 1 )  // 启用/禁用定时器

// 内存配置
#define UL_HEAP_SIZE        ((ul_size_t)(1024 * 10))   /* 堆大小 10KB */
```

## 2. 线程管理API

### 2.1 线程创建与初始化

#### ul_thread_create
```c
ul_thread_t* ul_thread_create(const char *name,
                              void (*entry)(void *p),
                              void *parameter,
                              ul_size_t stack_size,
                              ul_uint8_t priority,
                              ul_uint8_t time_slice);
```
- **功能**: 创建一个新线程
- **参数**:
  - `name`: 线程名称（最多24个字符）
  - `entry`: 线程执行函数
  - `parameter`: 传递给线程的参数
  - `stack_size`: 线程栈大小（字节）
  - `priority`: 线程优先级（0为最高优先级）
  - `time_slice`: 时间片长度
- **返回值**: 成功时返回线程控制块指针，失败时返回NULL

#### ul_thread_init
```c
ul_ecode ul_thread_init(struct ul_thread *self,
                        const char *name,
                        void (*entry)(void *p),
                        void* parameter,
                        void* stack_start,
                        ul_size_t stack_size,
                        ul_uint8_t priority,
                        ul_uint8_t time_slice);
```
- **功能**: 初始化静态分配的线程控制块
- **参数**: 与ul_thread_create类似，但使用预先分配的线程控制块
- **返回值**: 错误码（UL_EOK表示成功）

### 2.2 线程控制API

#### ul_thread_startup
```c
ul_ecode ul_thread_startup(struct ul_thread *self);
```
- **功能**: 启动一个已创建的线程
- **参数**: `self` - 线程控制块指针
- **返回值**: 错误码

#### ul_thread_delay
```c
void ul_thread_delay(ul_tick_t xTicksToDelay);
```
- **功能**: 使当前线程延时指定的系统tick数
- **参数**: `xTicksToDelay` - 延时的tick数

#### ul_thread_delay_until
```c
void ul_thread_delay_until(ul_tick_t *const pxPreviousWakeTime, 
                           const ul_tick_t xTimeIncrement);
```
- **功能**: 按固定周期延时线程
- **参数**: 
  - `pxPreviousWakeTime` - 上次唤醒时间的指针
  - `xTimeIncrement` - 时间增量

#### ul_thread_suspend & ul_thread_resume
```c
void ul_thread_suspend(struct ul_thread *self);  // 挂起线程
void ul_thread_resume(struct ul_thread *self);   // 恢复线程
```
- **功能**: 挂起和恢复指定线程

### 2.3 线程管理API

#### ul_thread_self
```c
ul_thread_t *ul_thread_self(void);
```
- **功能**: 获取当前线程的控制块指针
- **返回值**: 当前线程控制块指针

#### ul_thread_stack_remain
```c
ul_size_t ul_thread_stack_remain(ul_thread_t *thread);
```
- **功能**: 获取线程剩余栈空间
- **参数**: `thread` - 线程控制块指针
- **返回值**: 剩余栈空间大小（字节）

#### ul_thread_delete
```c
void ul_thread_delete(ul_thread_t *thread);
```
- **功能**: 删除指定线程

#### ul_thread_find
```c
ul_thread_t* ul_thread_find(const char *name);
```
- **功能**: 根据名称查找线程
- **参数**: `name` - 线程名称
- **返回值**: 找到的线程控制块指针，未找到返回NULL

### 2.4 线程优先级控制

#### ul_thread_control_set_priority
```c
ul_ecode ul_thread_control_set_priority(struct ul_thread *thread, ul_uint8_t priority);
```
- **功能**: 设置线程优先级
- **参数**: `thread` - 线程控制块指针，`priority` - 新的优先级值

#### ul_thread_control_get_priority
```c
ul_uint8_t ul_thread_control_get_priority(struct ul_thread *thread);
```
- **功能**: 获取线程当前优先级
- **参数**: `thread` - 线程控制块指针
- **返回值**: 线程当前优先级

#### ul_thread_control_restore_priority
```c
ul_ecode ul_thread_control_restore_priority(struct ul_thread *thread);
```
- **功能**: 恢复线程初始优先级

## 3. 内核管理API

### 3.1 内核初始化与调度

#### ul_kernel_init
```c
void ul_kernel_init(void);
```
- **功能**: 初始化ulOS内核

#### ul_scheduler_start
```c
void ul_scheduler_start(void);
```
- **功能**: 启动任务调度器（此函数不会返回）

#### ul_schedule
```c
void ul_schedule(void);
```
- **功能**: 手动触发任务调度

### 3.2 系统时钟API

#### ul_tick_increase
```c
void ul_tick_increase(void);
```
- **功能**: 系统时钟滴答增加（通常在定时器中断中调用）

#### ulOS_get_tick
```c
ul_tick_t ulOS_get_tick(void);
```
- **功能**: 获取当前系统时钟值
- **返回值**: 当前系统时钟值

## 4. IPC（进程间通信）API

### 4.1 消息队列API

#### ul_queue_create/ul_queue_init
```c
ul_queue_t* ul_queue_create(const char *name, uint16_t capacity, uint16_t msg_size);
ul_ecode ul_queue_init(ul_queue_t *queue, const char *name, void *buffer, uint16_t capacity, uint16_t msg_size);
```
- **功能**: 创建/初始化消息队列
- **参数**: `name` - 队列名称，`capacity` - 队列容量，`msg_size` - 消息大小

#### ul_queue_send/ul_queue_send_urgent
```c
ul_ecode ul_queue_send(ul_queue_t *queue, const void *buffer, ul_uint16_t len, ul_bool_t overwrite);
ul_ecode ul_queue_send_urgent(ul_queue_t *queue, const void *buffer, ul_uint16_t len, ul_bool_t overwrite);
```
- **功能**: 发送消息到队列（urgent版本发送到队列头部）

#### ul_queue_receive
```c
ul_ecode ul_queue_receive(ul_queue_t *queue, void *buffer, ul_uint16_t len, ul_tick_t timeout);
```
- **功能**: 从队列接收消息

#### ul_queue_available/ul_queue_available_space
```c
uint16_t ul_queue_available(ul_queue_t *queue);        // 获取队列中消息数量
uint16_t ul_queue_available_space(ul_queue_t *queue);  // 获取队列剩余空间
```

### 4.2 信号量API

#### ul_sem_create/ul_sem_init
```c
ul_sem_t* ul_sem_create(const char *name, ul_uint16_t init_count);
ul_ecode ul_sem_init(ul_sem_t *self, const char *name, ul_uint16_t init_count);
```
- **功能**: 创建/初始化信号量

#### ul_sem_take/ul_sem_give
```c
ul_ecode ul_sem_take(ul_sem_t *self, ul_tick_t timeout);  // 获取信号量
ul_ecode ul_sem_give(ul_sem_t *self);                     // 释放信号量
```

### 4.3 事件API

#### ul_event_create/ul_event_init
```c
ul_event_t* ul_event_create(const char *name);
ul_ecode ul_event_init(ul_event_t *event, const char *name);
```
- **功能**: 创建/初始化事件对象

#### ul_event_send/ul_event_recv
```c
ul_ecode ul_event_send(ul_event_t *event, uint32_t set);  // 发送事件
ul_ecode ul_event_recv(ul_event_t *event, uint32_t set, uint8_t option, ul_tick_t timeout, uint32_t *recved);
```
- **参数**:
  - `set` - 事件标志
  - `option` - 选项（UL_EVENT_FLAG_AND/OR/CLEAR）
  - `timeout` - 超时时间
  - `recved` - 接收到的事件

### 4.4 互斥锁API

#### ul_mutex_create/ul_mutex_init
```c
ul_mutex_t* ul_mutex_create(const char *name);
ul_ecode ul_mutex_init(struct ul_mutex *mutex, const char *name);
```
- **功能**: 创建/初始化互斥锁

#### ul_mutex_lock/ul_mutex_unlock
```c
ul_ecode ul_mutex_lock(struct ul_mutex *mutex, ul_tick_t timeout);  // 获取互斥锁
ul_ecode ul_mutex_unlock(struct ul_mutex *mutex);                   // 释放互斥锁
```

## 5. 临界区管理API

#### ul_enter_critical/ul_exit_critical
```c
void ul_enter_critical(void);  // 进入临界区
void ul_exit_critical(void);   // 退出临界区
```
- **功能**: 保护临界区，防止多线程同时访问共享资源

## 6. 错误码定义

```c
typedef enum {
    UL_EOK = 0,      // 操作成功
    UL_ERROR,        // 一般错误
    UL_ETIMEOUT,     // 超时错误
    UL_ENULL,        // 空指针错误
    UL_EFULL,        // 容器已满
    UL_EEMPTY,       // 容器为空
} ul_ecode;
```

## 7. 使用示例

### 7.1 基本线程创建和使用
```c
void thread_entry(void *p) {
    while(1) {
        // 线程执行代码
        ul_thread_delay(100);  // 延时100个tick
    }
}

int main(void) {
    ul_kernel_init();
    
    ul_thread_t *thread = ul_thread_create("example", 
                                           thread_entry, 
                                           NULL, 
                                           512,      // 栈大小
                                           1,        // 优先级
                                           1);       // 时间片
    ul_thread_startup(thread);
    
    ul_scheduler_start();
    return 0;
}
```

### 7.2 使用消息队列通信
```c
ul_queue_t *queue;

void sender_thread(void *p) {
    char msg[] = "Hello";
    while(1) {
        ul_queue_send(queue, msg, sizeof(msg), UL_FALSE);
        ul_thread_delay(1000);
    }
}

void receiver_thread(void *p) {
    char buffer[32];
    while(1) {
        if (ul_queue_receive(queue, buffer, sizeof(buffer), 100) == UL_EOK) {
            // 处理接收到的消息
        }
    }
}
```

### 7.3 使用互斥锁保护共享资源
```c
ul_mutex_t *mutex;
int shared_resource = 0;

void thread_func(void *p) {
    while(1) {
        ul_mutex_lock(mutex, ULOS_MAX_DELAY);  // 获取互斥锁
        // 访问共享资源
        shared_resource++;
        ul_mutex_unlock(mutex);  // 释放互斥锁
        ul_thread_delay(100);
    }
}
```