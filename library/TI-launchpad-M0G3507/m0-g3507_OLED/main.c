/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
// 一些可能用得上的辅助单片机运算的函数
#include "arm_math.h"
#include "stdio.h"
#include "string.h"
#include <ti/iqmath/include/IQmathLib.h>


#include "OLED/OLED.h"    //OLED屏幕文件
#include "System/Delay.h" //延时函数文件


int main(void) {
  SYSCFG_DL_init();
  OLED_Init();

  while (1) {
    // 中英文字符串混合显示
    OLED_NewFrame();
    OLED_PrintString(0, 0, "感谢关注", &font16x16, OLED_COLOR_REVERSED);
    OLED_ShowFrame();
    Delay_ms(500);
    OLED_PrintString(0, 22, "B站-KEYSKING", &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    Delay_ms(500);
    OLED_PrintString(0, 44, "\\^o^/", &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    Delay_ms(1500);
    // 显示变量值
    int count = 0;
    char buf[10] = {0};
    OLED_NewFrame();
    for (;;) {
      sprintf(buf, "%d %%", count);
      OLED_PrintASCIIString(40, 20, buf, &afont24x12, OLED_COLOR_NORMAL);
      OLED_ShowFrame();
      Delay_ms(15);
      if (count++ > 99) {
        break;
      }
    }
    Delay_ms(1000);
    // 直线绘制
    OLED_NewFrame();
    for (int i = 0; i < 128; i += 8) {
      OLED_DrawLine(0, 0, i, 63, OLED_COLOR_NORMAL);
      OLED_DrawLine(127, 0, 127 - i, 63, OLED_COLOR_NORMAL);
      OLED_ShowFrame();
      Delay_ms(30);
    }
    Delay_ms(1500);
    // 矩形绘制
    OLED_NewFrame();
    for (int i = 0; i < 64; i += 8) {
      OLED_DrawRectangle(i, i / 2, 127 - i * 2, 63 - i, OLED_COLOR_NORMAL);
      OLED_ShowFrame();
      Delay_ms(35);
    }
    Delay_ms(1500);
    // 矩形圆形
    OLED_NewFrame();
    for (int i = 63; i > 0; i -= 8) {
      OLED_DrawCircle(64, 32, i / 2, OLED_COLOR_NORMAL);
      OLED_ShowFrame();
      Delay_ms(40);
    }
    Delay_ms(1500);
    // 图片显示1
    OLED_NewFrame();
    OLED_DrawImage((128 - (bilibiliImg.w)) / 2, 0, &bilibiliImg,
                   OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    Delay_ms(1700);
    // 图片显示2
    OLED_NewFrame();
    OLED_DrawImage((128 - (bilibiliTVImg.w)) / 2, 0, &bilibiliTVImg,
                   OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    Delay_ms(1700);
  }
}