# STM32 中断驱动控制系统

基于 STM32F4 的中断驱动控制系统，实现串口通信、命令解析、事件驱动架构及多种外设控制。

## 功能特性

- **双串口通信**：USART1（主串口）和 USART3（蓝牙模块）
- **中断驱动**：基于串口接收中断实现命令接收
- **DFA 命令解析**：使用确定有限自动机（DFA）实现高效命令匹配
- **事件队列**：基于链表的事件驱动架构，支持异步事件处理
- **外设控制**：LED、蜂鸣器、电机、CO2 传感器、MQ-2 气体传感器

## 硬件需求

- STM32F4xx 开发板（如 STM32F系列开发板）
- 蓝牙模块（支持透传的串口蓝牙模块如DX-BT24）
- CO2 传感器（如 JW01）
- MQ-2 气体传感器
- LED 灯 × 4
- 蜂鸣器
- 直流电机及驱动模块（支持pwm调速的直流电机）
- 按键 × 若干

## 软件需求

- Keil MDK-ARM 5.x
- STM32F4xx 标准库
- CMSIS 核心库

## 项目结构

```
interrupt/
├── hardware/           # 硬件驱动模块
│   ├── uart.c/h       # 串口1驱动（命令接收）
│   ├── DX_BT24.c/h    # 蓝牙模块（串口3）
│   ├── led.c/h        # LED 驱动
│   ├── beep.c/h       # 蜂鸣器驱动
│   ├── motor.c/h      # 电机驱动
│   ├── CO2_sensor.c/h # CO2 传感器驱动
│   ├── mq_2.c/h       # MQ-2 气体传感器驱动
│   ├── key_interrupt.c/h # 按键中断驱动
│   └── timer.c/h      # 定时器驱动
├── system/            # 系统模块
│   ├── DFA_event_queue.c/h  # DFA命令解析和事件队列
│   ├── event_handlers.c/h   # 事件处理器
│   ├── pwm.c/h        # PWM控制
│   ├── retarget.c/h   # printf重定向到串口
│   └── systick.c/h    # 系统滴答定时器
├── .cmsis/            # CMSIS 核心库
├── Listings/          # 编译输出目录
└── interrupt.uvprojx  # Keil 项目文件
```

## 快速开始

### 1. 编译项目

1. 使用 Keil MDK-ARM 打开 `interrupt.uvprojx`
2. 选择目标设备为 STM32F4xx
3. 点击 Build 编译项目，生成 `interrupt.hex` 文件

### 2. 烧录程序

使用 ST-Link 或 J-Link 将 `interrupt.hex` 烧录到开发板。

### 3. 连接串口

通过 USART1 连接电脑串口助手：

- 波特率：115200
- 数据位：8
- 校验位：None
- 停止位：1
- 流控制：None

## 支持的命令

### LED 控制

```
格式：led-on/off-number（1-n）
led-on-1        # 打开 LED1
led1off-1       # 关闭 LED1
led-on-2        # 打开 LED2
led-off-2       # 关闭 LED2
led-on-3        # 打开 LED3
led-off-3       # 关闭 LED3
led-on-4        # 打开 LED4
led-off-4       # 关闭 LED4
```

### 蜂鸣器控制

```
格式：beep-on/off-number（1-n）
beep-on-1        # 打开蜂鸣器
beep-off-1       # 关闭蜂鸣器
```

### 电机控制

```
格式：fan-on/off-number（1-n）
fan-on-1         # 打开风扇/电机
fan-off-1        # 关闭风扇/电机
格式：fan-speed-percentage（0-100）
fan-speed-50     # 设置电机速度为 50%（参数范围：0-100）
```

### 传感器查询

```
co2-1            # 查询 CO2 浓度（ppm）
co2-auto       # 切换到 CO2 浓度调节通风程度
co2-off        # 关闭 CO2 传感器自动控制模式
```

## 命令格式说明

- 终端输入的所有命令必须以 `\r\n`（CRLF）结尾
- 命令必须严格按照格式输入
- 参数必须紧跟命令，中间无空格
- 命令之间字符无限制，一条命令可以写入多条控制指令

## 架构设计

### 中断处理流程

```
USART1_IRQHandler → DFA_Match_Byte → Event_Push → Event_Pop → Handler
    ↓
串口接收字节      DFA命令匹配      入队事件      出队执行     执行具体动作
```

### DFA 命令匹配原理

系统使用确定有限自动机（DFA）逐字节匹配命令：

1. 维护每个命令的匹配进度
2. 接收到字符后更新所有命令的匹配状态
3. 匹配完成后进入参数收集或直接触发事件
4. 匹配失败则重置对应命令的进度

### 事件队列机制

采用链表实现事件队列：

- 支持动态内存分配
- 事件入队和出队操作
- 支持带参数的事件处理

## 硬件连接参考

| 外设        | STM32 引脚 | 说明        |
| --------- | -------- | --------- |
| LED1      | PA0      | 推挽输出      |
| LED2      | PA1      | 推挽输出      |
| LED3      | PA2      | 推挽输出      |
| LED4      | PA3      | 推挽输出      |
| 蜂鸣器       | PA4      | 推挽输出      |
| 电机 PWM    | PC6      | TIM3\_CH1 |
| USART1 TX | PA9      |        |
| USART1 RX | PA10     |        |
| CO2 传感器 | usart2     | 复用功能（PA2，PA3） |
| USART3 TX | PB10     |        |
| USART3 RX | PB11     |        |
| 蓝牙模块 | usart3     | 复用功能（PB10，PB11） |

## 许可证

MIT License

## 作者

本项目为 STM32 嵌入式学习项目。

***

**注意**：使用前请确保正确连接硬件，避免短路损坏设备。
