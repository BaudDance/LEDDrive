#include <Arduino.h>
#include "oled.h"
#include <Wire.h>

int i = 0;
char msg[50] = { 0 };

void setup() {
    Serial.begin(115200);   // 初始化串口速率
    Wire.begin(8, 9);       // 初始化I2C引脚
    Wire.setClock(400000);
    delay(200);
    OLED_Init();

    OLED_NewFrame();
    OLED_DrawImage(40, 16, &bilibiliImg, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    delay(2000);

}

void loop() {

    OLED_NewFrame();
    i++;
    sprintf(msg, "time:%d", i);
    OLED_PrintString(0, 0, msg, &font16x16, OLED_COLOR_NORMAL);
    OLED_PrintString(0, 17, "波特律动hello", &font16x16, OLED_COLOR_REVERSED);
    OLED_PrintASCIIString(0, 34, "I AM THE STORM ", &afont12x6, OLED_COLOR_NORMAL);
    OLED_PrintASCIIString(0, 50, "THAT IS APPROACHING!!", &afont12x6, OLED_COLOR_NORMAL);
    OLED_ShowFrame();

    delay(100);

}
