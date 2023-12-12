## 使用方法

| 由baud-dance基于STM32 HAL库的OLED驱动改造而来
| 适用STC15,STC8等具有硬件I2C, flash >= 16kB, sram >= 2kB的8051核MCU
| 本库适配SSD1306/SSD1315等系列芯片
| 编译环境：
IDE-Version:
μVision V5.38.0.0
Copyright (C) 2022 ARM Ltd and ARM Germany GmbH. All rights reserved.

License Information:
L FQ Lee
xxxxx
LIC=xxxxx-xxxxx-xxxxx-xxxxx-xxxxx-xxxxx

Tool Version Numbers:
Toolchain:        PK51 Prof. Developers Kit  Version: 9.60.7.0
Toolchain Path:    D:\ProgramFiles\Keil_v5\C51\BIN
C Compiler:         C51.exe    V9.60.7.0
Assembler:          A51.exe    V8.2.7.0
Linker/Locator:     BL51.exe    V6.22.4.0
Librarian:             LIB51.exe    V4.30.1.0
Hex Converter:      OH51.exe    V2.7.0.0
CPU DLL:               S8051.DLL            V3.125.1.0
Dialog DLL:         TP51.DLL             V2.60.1.0
Target DLL:             STCMON51.DLL         V0, 1, 0, 17
Dialog DLL:         TP51.DLL             V2.60.1.0

### 0. 工程配置
示例工程默认使用的是STC8H8K64U的硬件I2C.
编译选项Memory Model选择Large:variables in XDATA.
下载时主频设置为24MHz，其他选项保持默认.
默认情况下SDA位于P1.4，SCL位于P1.5.
如有软件I2C或SPI需求, 需修改`oled.h`中的Send函数和相应通信库函数.

### 1. 引入
#### CubeIDE
将本库的`oled.c`, `font.c`,`oled.h`, `font.h`,`STC8G_H_I2C.c`,`STC8G_H_I2C.h`,`STC8H.h`或`STC8G.h`文件加入C51工程源文件夹.
将`oled.c`, `font.c`,`STC8G_H_I2C.c`加入工程.
(与STC官方I2C库函数相比，工程中使用的库增添了void I2C_WriteNbyte_WithoutmemaddrWrite(u8 dev_addr, u8 *p, u8 number)
和void I2C_ReadNbyte_WithoutmemaddrWrite(u8 dev_addr, u8 *p, u8 number)两个函数，特别注意)

### 2. 使用
1. STC8初始化GPIO和IIC完成后调用`OLED_Init()`初始化OLED. 注意STC8启动比OLED上电快, 可等待100ms再初始化OLED
2. 调用`OLED_NewFrame()`开始绘制新的一帧
3. 调用`OLED_DrawXXX()`系列函数绘制图形到显存 调用`OLED_Printxxx()`系列函数绘制文本到显存
4. 调用`OLED_ShowFrame()`将显存内容显示到OLED

```c
OLED_Init();

while(1){
    OLED_NewFrame();
    OLED_PrintString(i, i, "波特律动hello", &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    HAL_Delay(100);
}
```