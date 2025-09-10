## 使用方法

| 本库适配SSD1306芯片，基于ESP32-S3开发板，其他芯片和开发板没有详细测试，理论上适配所有ESP开发板


### 0. 工程配置
示例工程默认使用的是ESP32-S3的I2C0，且配置为8-SDA，9-SCL，为方便开发，I2C初始化使用了 Wire 库，命令发送使用原生 IDF 调用（以绕过 Wire 库的 32 字节 TX 缓冲区硬上限）
如果需要使用其他引脚，可以修改Wire.begin(8, 9);

### 1. 引入
#### Platform
找到本工程的lib文件夹，将其中的oled文件夹复制到你工程中的lib文件夹中，若使用CLion的Platform插件，还需要重新加载项目

### 2. 使用
1. 在setup中初始化I2C,可以使用 Wire 库的 Wire.begin 和 Wire.setClock
2. 调用`OLED_NewFrame()`开始绘制新的一帧
3. 调用`OLED_DrawXXX()`系列函数绘制图形到显存 调用`OLED_Printxxx()`系列函数绘制文本到显存
4. 调用`OLED_ShowFrame()`将显存内容显示到OLED

```c
void setup() {
    Wire.begin(8, 9);       // 初始化I2C引脚，此处可以按需求设置
    Wire.setClock(400000);  // 初始化I2C速率
    delay(200);
    OLED_Init();
}

void loop() {
    OLED_NewFrame();
    OLED_PrintString(0, 17, "波特律动hello", &font16x16, OLED_COLOR_REVERSED);
    OLED_ShowFrame();
    delay(100);
}
```
