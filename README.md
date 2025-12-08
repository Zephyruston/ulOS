# ulOS - 轻量级RTOS

## 简介

轻量RTOS，提供了任务调度、同步机制、通信机制和定时器等基本功能，并提供各种组件。

## 主要特性

- **多任务调度**：支持抢占式优先级调度+可编辑的时间片调度，无优先级数量限制
- **同步机制**：提供互斥锁(Mutex)用于资源保护
- **通信机制**：支持信号量(Semaphore)、队列(Queue)和事件(Event)等IPC机制
- **定时器服务**：提供软件定时器功能
- **内存管理**：内置小内存堆管理
- **可裁剪性**：通过配置文件可以灵活裁剪功能，减少资源占用

## 系统架构

```
ulOS/
├── kernel/              # 内核核心代码
│   ├── inc/            # 内核头文件
│   └── src/            # 内核源文件
├── component/          # 组件代码
├── example/            # 示例代码
├── platform/           # 平台相关代码
└── ul_config.h         # 系统配置文件
```

## 组件库

ulOS提供了丰富的组件库，方便开发者快速构建应用：

### 硬件驱动组件

- **ul_pin**: GPIO引脚控制，支持输入/输出模式配置和电平读写
- **ul_button**: 按键驱动，支持防抖处理和事件回调
- **ul_buzzer**: 蜂鸣器控制，支持不同频率和模式的蜂鸣
- **ul_softwarei2c**: 软件模拟I2C总线，无需硬件I2C资源
- **ul_softwarespi**: 软件模拟SPI总线，支持全双工通信
- **ul_at24cxx**: AT24C系列EEPROM存储器驱动
- **ul_w25qxx**: W25Q系列SPI Flash存储器驱动
- **ul_max6675**: MAX6675热电偶温度传感器驱动
- **ul_ina226**: INA226高精度电流/功率传感器驱动

### 算法与工具组件

- **ul_filter**: 数字滤波器，包括均值滤波、中值滤波等常用算法
- **ul_crc**: CRC校验算法，支持多种标准CRC计算
- **ul_pid**: PID控制器实现，支持位置式和增量式PID
- **ul_swtimer**: 软件定时器，提供毫秒级精度的非阻塞延时

### 通信组件

- **ul_topic**: 发布-订阅模式通信组件，支持模块间解耦通信

这些组件设计为可独立使用，也可以与ulOS内核无缝集成，提供了丰富的功能支持，大大简化了嵌入式应用的开发过程。

# 快速开始

这里是简单创建移植工程的教程，点击跳转 [移植工程示例](docs/transplant%20guide/stm32/example-f103.md)

## API参考

### 线程管理

```c
// 创建线程
ul_thread_t *ul_thread_create(const char *name, void (*entry)(void *), void *param, 
                              ul_size_t stack_size, ul_uint8_t priority, ul_uint8_t timeslice);

// 启动线程
ul_ecode ul_thread_startup(ul_thread_t *thread);

// 延时当前线程
ul_ecode ul_thread_delay(ul_tick_t ticks);
```

### 互斥锁

```c
// 创建互斥锁
ul_mutex_t *ul_mutex_create(void);

// 获取互斥锁
ul_ecode ul_mutex_lock(ul_mutex_t *mutex, ul_tick_t timeout);

// 释放互斥锁
ul_ecode ul_mutex_unlock(ul_mutex_t *mutex);
```

### 信号量

```c
// 创建信号量
ul_sem_t *ul_sem_create(ul_uint32_t initial_count, ul_uint32_t max_count);

// 获取信号量
ul_ecode ul_sem_wait(ul_sem_t *sem, ul_tick_t timeout);

// 释放信号量
ul_ecode ul_sem_post(ul_sem_t *sem);
```

### 队列

```c
// 创建队列
ul_queue_t *ul_queue_create(ul_size_t msg_size, ul_size_t max_msgs);

// 发送消息
ul_ecode ul_queue_send(ul_queue_t *queue, void *msg, ul_tick_t timeout);

// 接收消息
ul_ecode ul_queue_recv(ul_queue_t *queue, void *msg, ul_tick_t timeout);
```

### 定时器

```c
// 创建定时器
ul_timer_t *ul_timer_create(const char *name, void (*timeout_func)(void *), 
                            void *param, ul_tick_t tick, ul_bool_t is_periodic);

// 启动定时器
ul_ecode ul_timer_start(ul_timer_t *timer);

// 停止定时器
ul_ecode ul_timer_stop(ul_timer_t *timer);
```

## 系统配置

ulOS的配置通过修改 `ulOS/ul_config.h` 文件实现：

```c
// 最大优先级
#define ULOS_CONFIG_MAX_PRIORITY    (8)

// 功能开关
#define ULOS_CONFIG_USE_MUTEX          (1)
#define ULOS_CONFIG_USE_QUEUE          (1)
#define ULOS_CONFIG_USE_SEMAPHORE      (1)
#define ULOS_CONFIG_USE_EVENT          (1)
#define ULOS_CONFIG_USE_TIMER          (1)

// 内存配置
#define UL_HEAP_SIZE        ((ul_size_t)(1024 * 10))   /* 堆大小 10KB */
```

## 移植指南

参考[ulOS\docs\transplant guide\stm32](docs/transplant%20guide/stm32/) 目录下的Cortex-M3平台实现。

## 许可证

本项目采用MIT许可证，详见LICENSE文件。

## 如何贡献

1. Fork 项目
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建 Pull Request

## 联系方式

如有问题或建议，请通过以下方式联系：

| 项目维护者 | 邮箱 |
|-----------|------|
| David Chu | 2499311743@qq.com |
| levi_jia | 13533319308@163.com |

- 项目地址：[https://gitee.com/David_chu1119/ulOS]

