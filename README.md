# STM32 LVGL 显示驱动与应用 Demo

基于 STM32F4 的 LVGL（Light and Versatile Graphics Library）图形库移植与应用示例。

## 项目概述

本项目展示了如何在 STM32F4 开发板上移植和使用 LVGL 图形库，实现高性能的嵌入式 GUI 界面。

## 功能特性

- **LVGL 图形库**：移植 LVGL 到 STM32F4 平台
- **TFT LCD 驱动**：支持多种 TFT LCD 显示屏（如 ILI9341）
- **触摸屏支持**：电阻式触摸屏驱动与校准
- **中文字库**：内置 16x16、24x24、32x32 点阵中文字库
- **图形绘制**：支持矩形、圆形、三角形等基本图形
- **图片显示**：支持 BMP 格式图片显示
- **显示旋转**：支持 0°/90°/180°/270° 屏幕旋转

## 硬件需求

- STM32F4xx 开发板（如 STM32F407ZGT6）
- TFT LCD 显示屏（推荐 2.8" ILI9341 240x320）
- 电阻式触摸屏（可选）
- ST-Link 或 J-Link 调试器

## 软件需求

- Keil MDK-ARM 5.x
- STM32F4xx 标准库
- CMSIS 核心库
- LVGL 图形库

## 项目结构

```
LVGL/
├── LCD/                    # LCD 显示模块
│   ├── app/                # LVGL 应用层
│   │   ├── test.c/h        # LVGL 测试程序
│   │   └── test1.c/h       # 附加测试程序
│   └── driver/             # LCD 驱动层
│       ├── lcd.c/h         # LCD 驱动（FSMC/SPI）
│       ├── touch.c/h       # 触摸屏驱动
│       ├── FONT.H          # 中文字库（宋体常规小四）
│       ├── myiic.c/h       # I2C 驱动
│       └── pic.h           # 图片数据
├── lvgl/                   # LVGL 图形库（需自行添加）
│   ├── lv_conf.h           # LVGL 配置文件
│   ├── lv_core/            # LVGL 核心模块
│   ├── lv_obj/             # LVGL 对象模块
│   └── lv_drivers/         # LVGL 驱动模块
├── hardware/               # 硬件驱动
│   ├── SPI.c/h             # SPI 驱动
│   ├── key_interrupt.c/h   # 按键中断
│   └── timer.c/h           # 定时器驱动
├── system/                 # 系统模块
│   ├── systick.c/h         # 系统滴答定时器
│   └── retarget.c/h        # printf 重定向
└── interrupt.uvprojx       # Keil 项目文件
```

## 快速开始

### 1. 获取 LVGL 库

1. 从 [LVGL 官网](https://lvgl.io/) 下载最新版本
2. 将 `lvgl` 目录复制到项目根目录
3. 根据硬件配置修改 `lvgl/lv_conf.h`：
   - 设置屏幕分辨率 `LV_HOR_RES_MAX` 和 `LV_VER_RES_MAX`
   - 配置颜色深度 `LV_COLOR_DEPTH`（推荐 16 位）
   - 启用必要的组件

### 2. 编译项目

1. 使用 Keil MDK-ARM 打开 `interrupt.uvprojx`
2. **编码设置**：由于项目包含中文字库，需确保编码一致性：
   - 在 Keil 中点击 `Edit → Configuration → Editor`
   - 将 `Encoding` 设置为 `Chinese GB2312 (Simplified)`
   - 或使用 `UTF-8 with BOM` 格式
3. 配置项目路径，确保 LVGL 库路径正确
4. 点击 Build 编译项目

### 3. 烧录与运行

1. 使用 ST-Link 将生成的 `.hex` 文件烧录到开发板
2. 上电后将自动运行 LVGL 测试程序

## LVGL 移植说明

### 显示驱动配置

在 `LCD/driver/lcd.c` 中实现 LVGL 的显示刷新回调：

```c
void lv_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    lv_area_t a = *area;  // 复制到本地可变副本
    // 调用底层 LCD 刷新函数
    LCD_Fill(a.x1, a.y1, a.x2, a.y2, (u16*)color_p);
    lv_disp_flush_ready(disp);  // 通知 LVGL 刷新完成
}
```

### 触摸屏驱动配置

在 `LCD/driver/touch.c` 中实现 LVGL 的输入设备回调：

```c
bool lv_touch_read(lv_indev_drv_t *indev, lv_indev_data_t *data)
{
    // 读取触摸坐标
    if(tp_dev.sta & TP_PRES_DOWN) {
        data->point.x = tp_dev.x;
        data->point.y = tp_dev.y;
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
    return false;  // 没有更多数据
}
```

### 定时器配置

在 `system/systick.c` 中配置 LVGL 时基：

```c
void SysTick_Handler(void)
{
    HAL_IncTick();
    lv_tick_inc(1);  // LVGL 1ms 时基
}
```

## 测试程序说明

### main_test() - 主界面测试

显示项目信息和欢迎界面。

### Chinese_Font_test() - 中文字库测试

测试不同字号的中文显示：
- 16x16 中文显示
- 24x24 中文显示  
- 32x32 中文显示

### Touch_Test() - 触摸屏测试

测试触摸功能和屏幕校准。

### Rotate_Test() - 屏幕旋转测试

测试 0°/90°/180°/270° 四种旋转模式。

## 编码注意事项

- **中文字库编码**：`LCD/driver/FONT.H` 使用 GB2312 编码
- **文件编码一致性**：所有包含中文的源文件需使用相同编码
- **UTF-8 支持**：如需使用 UTF-8，必须带 BOM 标记
- **编译错误**：编码不一致可能导致 `missing closing quote` 等错误

## 许可证

MIT License

## 参考资料

- [LVGL 官方文档](https://docs.lvgl.io/)
- [STM32F4 官方手册](https://www.st.com/en/microcontrollers-microprocessors/stm32f407zgt6.html)
- [LCD Wiki](http://www.lcdwiki.com/)

---

**注意**：使用前请确保正确连接硬件，避免短路损坏设备。
