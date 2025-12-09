# ulOS - 轻量级RTOS

## 简介

微内核，提供线程调度、同步机制、通信机制和定时器等基本功能，并提供各种组件。最小占用ROM >= 3KB，RAM >= 1KB

## 主要特性

- **多线程调度**：支持抢占式优先级调度+可编辑的时间片调度，无优先级数量限制
- **同步机制**：提供互斥锁(Mutex)用于资源保护
- **通信机制**：支持信号量(Semaphore)、队列(Queue)和事件(Event)等IPC机制
- **定时器服务**：提供软件定时器功能
- **内存管理**：集成空闲内存块合并算法的堆管理模块
- **内核裁剪**：通过配置文件可以灵活裁剪功能，减少资源占用

## 系统架构

```
ulOS/
├── kernel              # 内核实现
├── component           # 组件
├── example             # 示例代码
├── platform            # 平台移植实现
├── docs                # 文档集合
├── LICENSE             # 许可证
└── ul_config.h         # 配置文件
```

## 组件库

均采用桥接模式实现，方便开发者快速构建应用：

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

这里是简单创建移植工程的教程，点击跳转 [移植工程示例](docs/transplant_guide/stm32_hal/example-f103.md)

## API

请参考 [ulOS API使用参考手册](docs/api/README.md)

## 移植指南

参考[ulOS\docs\transplant_guide\stm32](docs/transplant_guide/stm32_hal/) 目录下的Cortex-M3平台实现。

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

