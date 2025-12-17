<p align="center">
<img src="docs/pictures/logo.png" width="60%" >
</p>



# ulOS - 嵌入式实时操作系统

Just for fun.	:-)

---

轻量嵌入式RTOS，提供线程调度、同步、通信和定时器等基本功能，并提供各种组件。

最小占用ROM >= 3KB，RAM >= 1KB（-O2）

## 移植指南，例程

点击跳转 [移植工程示例](docs/transplant_guide/stm32_hal/example-f103.md)

## 特性

- **优先级机制**: 0为最高优先级，最大32个优先级
- **多线程调度**：支持抢占式优先级调度+可编辑的时间片调度
- **同步机制**：提供互斥锁(Mutex)用于资源保护
- **通信机制**：支持信号量(Semaphore)、队列(Queue)和事件(Event)等IPC机制
- **定时器服务**：提供软件定时器功能
- **内存管理**：集成空闲内存块合并算法的堆管理模块
- **内核裁剪**：通过配置文件可以灵活裁剪功能，减少资源占用

## 架构

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

均采用桥接模式实现抽象，方便开发者快速构建应用：

**硬件驱动组件**

- **ul_pin**: 常用于LED，蜂鸣器等PIN控制，可实现无阻塞指令式控制
- **ul_button**: 按键驱动，支持防抖处理和事件回调
- **ul_buzzer**: 蜂鸣器控制，支持不同频率和模式的蜂鸣
- **ul_softwarei2c**: 软件模拟I2C总线，无需硬件I2C资源
- **ul_softwarespi**: 软件模拟SPI总线，支持全双工通信
- **ul_at24cxx**: AT24C系列EEPROM存储器驱动
- **ul_w25qxx**: W25Q系列SPI Flash存储器驱动
- **ul_max6675**: MAX6675热电偶温度传感器驱动
- **ul_ina226**: INA226高精度电流/功率传感器驱动

**算法与工具组件**

- **ul_filter**: 数字滤波器，包括均值滤波、中值滤波等常用算法
- **ul_crc**: CRC校验算法，支持多种标准CRC计算
- **ul_pid**: PID控制器实现，支持位置式和增量式PID
- **ul_swtimer**: 软件定时器，提供毫秒级精度的非阻塞延时

**通信组件**

- **ul_topic**: 发布-订阅模式通信组件，支持模块间解耦通信

## API

请参考 [ulOS API使用参考手册](docs/api/README.md)

## 许可证

本项目采用MIT许可证，详见LICENSE文件。

## 如何贡献

1. Fork 项目
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建 Pull Request

### TODO:

- [ ] 提供cmsis-os2抽象层
- [ ] 添加makefile
- [ ] 添加编码规范文档
- [ ] 内核加入QPC架构
- [ ] 持续丰富组件库
- [ ] 支持更多第三方库
- [ ] 实现极简的GUI框架
- [ ] 支持更多平台
- [ ] 支持单元测试
- [ ] 添加实时性能分析和监控工具
- [ ] 支持日志系统
- [ ] 支持图形化配置工具
- [ ] 实现低功耗管理框架

## 联系方式

如有问题或建议，请通过以下方式联系：

| 项目维护者 | email               |
|-----------|------|
| David Chu | 2499311743@qq.com |
| levi_jia | 13533319308@163.com |

项目地址：

gitee: https://gitee.com/David_chu1119/ulOS

github: https://gitee.com/David_chu1119/ulOS

