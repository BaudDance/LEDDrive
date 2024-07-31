

## 使用方法

本库适配SSD1306/SSD1315等系列芯片，IIC驱动仅支持TI-MSPM0G3507或者其他TI板子，没有详细测试。

### 0. 工程配置

示例工程默认使用的是CCS-THEIA的I2C0.
如有其他I2C或SPI需求, 需修改CCS-theia与 `oled.h`中的Send函数即可完成移植.

### 1. 引入

#### CCS-theia

将本库的 `oled.c`, `font.c`, `oled.h`, `font.h`文件放入随便一个文件夹.

### 2. 使用

本程序引脚占用：

PA0->SDA(I2C)

PA1->SCL(I2C)

程序部分均正常，敬请参考其他文件
