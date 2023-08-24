/**
 * @file oled.c
 * @brief 波特律动OLED驱动(SSD1306)
 * @anchor 波特律动(keysking 博哥在学习)
 * @version 1.0
 * @date 2023-08-19
 * @license MIT License
 *
 * @attention
 * 本驱动库针对波特律动·keysking的STM32教程学习套件进行开发
 * 在其他平台或驱动芯片上使用可能需要进行移植
 *
 * @note
 * 使用流程:
 * 1. STM32初始化IIC完成后调用OLED_Init()初始化OLED. 注意STM32启动比OLED上电快, 可等待20ms再初始化OLED
 * 2. 调用OLED_NewFrame()开始绘制新的一帧
 * 3. 调用OLED_DrawXXX()系列函数绘制图形到显存 调用OLED_Printxxx()系列函数绘制文本到显存
 * 4. 调用OLED_ShowFrame()将显存内容显示到OLED
 *
 * @note
 * 为保证中文显示正常 请将编译器的字符集设置为UTF-8
 *
 */
#include "oled.h"
#include "i2c.h"
#include <math.h>
#include <stdlib.h>

// OLED器件地址
#define OLED_ADDRESS 0x78

// OLED参数
#define OLED_PAGE 8            // OLED页数
#define OLED_ROW 8 * OLED_PAGE // OLED行数
#define OLED_COLUMN 128        // OLED列数

// 显存
uint8_t OLED_GRAM[OLED_PAGE][OLED_COLUMN];

// ========================== 底层通信函数 ==========================

/**
 * @brief 向OLED发送数据的函数
 * @param data 要发送的数据
 * @param len 要发送的数据长度
 * @return None
 * @note 此函数是移植本驱动时的重要函数 将本驱动库移植到其他平台时应根据实际情况修改此函数
 */
void OLED_Send(uint8_t *data, uint8_t len)
{
  HAL_I2C_Master_Transmit(&hi2c2, OLED_ADDRESS, data, len, HAL_MAX_DELAY);
}

/**
 * @brief 向OLED发送指令
 */
void OLED_SendCmd(uint8_t cmd)
{
  static uint8_t sendBuffer[2] = {0};
  sendBuffer[1] = cmd;
  OLED_Send(sendBuffer, 2);
}

// ========================== OLED驱动函数 ==========================

/**
 * @brief 初始化OLED (SSD1306)
 * @note 此函数是移植本驱动时的重要函数 将本驱动库移植到其他驱动芯片时应根据实际情况修改此函数
 */
void OLED_Init()
{
  OLED_SendCmd(0xAE); /*关闭显示 display off*/

  OLED_SendCmd(0x20);
  OLED_SendCmd(0x10);

  OLED_SendCmd(0xB0);

  OLED_SendCmd(0xC8);

  OLED_SendCmd(0x00);
  OLED_SendCmd(0x10);

  OLED_SendCmd(0x40);

  OLED_SendCmd(0x81);

  OLED_SendCmd(0xDF);
  OLED_SendCmd(0xA1);

  OLED_SendCmd(0xA6);
  OLED_SendCmd(0xA8);

  OLED_SendCmd(0x3F);

  OLED_SendCmd(0xA4);

  OLED_SendCmd(0xD3);
  OLED_SendCmd(0x00);

  OLED_SendCmd(0xD5);
  OLED_SendCmd(0xF0);

  OLED_SendCmd(0xD9);
  OLED_SendCmd(0x22);

  OLED_SendCmd(0xDA);
  OLED_SendCmd(0x12);

  OLED_SendCmd(0xDB);
  OLED_SendCmd(0x20);

  OLED_SendCmd(0x8D);
  OLED_SendCmd(0x14);

  OLED_NewFrame();
  OLED_ShowFrame();

  OLED_SendCmd(0xAF); /*开启显示 display ON*/
}

/**
 * @brief 开启OLED显示
 */
void OLED_DisPlay_On()
{
  OLED_SendCmd(0x8D); // 电荷泵使能
  OLED_SendCmd(0x14); // 开启电荷泵
  OLED_SendCmd(0xAF); // 点亮屏幕
}

/**
 * @brief 关闭OLED显示
 */
void OLED_DisPlay_Off()
{
  OLED_SendCmd(0x8D); // 电荷泵使能
  OLED_SendCmd(0x10); // 关闭电荷泵
  OLED_SendCmd(0xAE); // 关闭屏幕
}

/**
 * @brief 设置颜色模式 黑底白字或白底黑字
 * @param ColorMode 颜色模式COLOR_NORMAL/COLOR_REVERSED
 * @note 此函数直接设置屏幕的颜色模式
 */
void OLED_SetColorMode(OLED_ColorMode mode)
{
  if (mode == OLED_COLOR_NORMAL)
  {
    OLED_SendCmd(0xA6); // 正常显示
  }
  if (mode == OLED_COLOR_REVERSED)
  {
    OLED_SendCmd(0xA7); // 反色显示
  }
}

// ========================== 显存操作函数 ==========================

/**
 * @brief 清空显存 绘制新的一帧
 */
void OLED_NewFrame()
{
  memset(OLED_GRAM, 0, sizeof(OLED_GRAM));
}

/**
 * @brief 将当前显存显示到屏幕上
 * @note 此函数是移植本驱动时的重要函数 将本驱动库移植到其他驱动芯片时应根据实际情况修改此函数
 */
void OLED_ShowFrame()
{
  static uint8_t sendBuffer[OLED_COLUMN + 1];
  sendBuffer[0] = 0x40;
  for (uint8_t i = 0; i < OLED_PAGE; i++)
  {
    OLED_SendCmd(0xB0 + i); // 设置页地址
    OLED_SendCmd(0x00);     // 设置列地址低4位
    OLED_SendCmd(0x10);     // 设置列地址高4位
    memcpy(sendBuffer + 1, OLED_GRAM[i], OLED_COLUMN);
    OLED_Send(sendBuffer, OLED_COLUMN + 1);
  }
}

/**
 * @brief 设置一个像素点
 * @param x 横坐标
 * @param y 纵坐标
 * @param color 颜色
 */
void OLED_SetPixel(uint8_t x, uint8_t y, OLED_ColorMode color)
{
  if (x >= OLED_COLUMN || y >= OLED_ROW)
    return;
  if (!color)
  {
    OLED_GRAM[y / 8][x] |= 1 << (y % 8);
  }
  else
  {
    OLED_GRAM[y / 8][x] &= ~(1 << (y % 8));
  }
}

/**
 * @brief 设置显存中一字节数据的某几位
 * @param page 页地址
 * @param column 列地址
 * @param data 数据
 * @param start 起始位
 * @param end 结束位
 * @param color 颜色
 * @note 此函数将显存中的某一字节的第start位到第end位设置为与data相同
 * @note start和end的范围为0-7, start必须小于等于end
 * @note 此函数与OLED_SetByte_Fine的区别在于此函数只能设置显存中的某一真实字节
 */
void OLED_SetByte_Fine(uint8_t page, uint8_t column, uint8_t data, uint8_t start, uint8_t end, OLED_ColorMode color)
{
  static uint8_t temp;
  if (page >= OLED_PAGE || column >= OLED_COLUMN)
    return;
  if (color)
    data = ~data;

  temp = data | (0xff << (end + 1)) | (0xff >> (8 - start));
  OLED_GRAM[page][column] &= temp;
  temp = data & ~(0xff << (end + 1)) & ~(0xff >> (8 - start));
  OLED_GRAM[page][column] |= temp;
  // 使用OLED_SetPixel实现
  // for (uint8_t i = start; i <= end; i++) {
  //   OLED_SetPixel(column, page * 8 + i, !((data >> i) & 0x01));
  // }
}

/**
 * @brief 设置显存中的一字节数据
 * @param page 页地址
 * @param column 列地址
 * @param data 数据
 * @param color 颜色
 * @note 此函数将显存中的某一字节设置为data的值
 */
void OLED_SetByte(uint8_t page, uint8_t column, uint8_t data, OLED_ColorMode color)
{
  if (page >= OLED_PAGE || column >= OLED_COLUMN)
    return;
  if (color)
    data = ~data;
  OLED_GRAM[page][column] = data;
}

/**
 * @brief 设置显存中的一字节数据的某几位
 * @param x 横坐标
 * @param y 纵坐标
 * @param data 数据
 * @param len 位数
 * @param color 颜色
 * @note 此函数将显存中从(x,y)开始向下数len位设置为与data相同
 * @note len的范围为1-8
 * @note 此函数与OLED_SetByte_Fine的区别在于此函数的横坐标和纵坐标是以像素为单位的, 可能出现跨两个真实字节的情况(跨页)
 */
void OLED_SetBits_Fine(uint8_t x, uint8_t y, uint8_t data, uint8_t len, OLED_ColorMode color)
{
  uint8_t page = y / 8;
  uint8_t bit = y % 8;
  if (bit + len > 8)
  {
    OLED_SetByte_Fine(page, x, data << bit, bit, 7, color);
    OLED_SetByte_Fine(page + 1, x, data >> (8 - bit), 0, len + bit - 1 - 8, color);
  }
  else
  {
    OLED_SetByte_Fine(page, x, data << bit, bit, bit + len - 1, color);
  }
  // 使用OLED_SetPixel实现
  // for (uint8_t i = 0; i < len; i++) {
  //   OLED_SetPixel(x, y + i, !((data >> i) & 0x01));
  // }
}

/**
 * @brief 设置显存中一字节长度的数据
 * @param x 横坐标
 * @param y 纵坐标
 * @param data 数据
 * @param color 颜色
 * @note 此函数将显存中从(x,y)开始向下数8位设置为与data相同
 * @note 此函数与OLED_SetByte的区别在于此函数的横坐标和纵坐标是以像素为单位的, 可能出现跨两个真实字节的情况(跨页)
 */
void OLED_SetBits(uint8_t x, uint8_t y, uint8_t data, OLED_ColorMode color)
{
  uint8_t page = y / 8;
  uint8_t bit = y % 8;
  OLED_SetByte_Fine(page, x, data << bit, bit, 7, color);
  if (bit)
  {
    OLED_SetByte_Fine(page + 1, x, data >> (8 - bit), 0, bit - 1, color);
  }
}

/**
 * @brief 设置一块显存区域
 * @param x 起始横坐标
 * @param y 起始纵坐标
 * @param data 数据的起始地址
 * @param w 宽度
 * @param h 高度
 * @param color 颜色
 * @note 此函数将显存中从(x,y)开始的w*h个像素设置为data中的数据
 * @note data的数据应该采用列行式排列
 */
void OLED_SetBlock(uint8_t x, uint8_t y, const uint8_t *data, uint8_t w, uint8_t h, OLED_ColorMode color)
{
  uint8_t fullRow = h / 8; // 完整的行数
  uint8_t partBit = h % 8; // 不完整的字节中的有效位数
  for (uint8_t i = 0; i < w; i++)
  {
    for (uint8_t j = 0; j < fullRow; j++)
    {
      OLED_SetBits(x + i, y + j * 8, data[i + j * w], color);
    }
  }
  if (partBit)
  {
    uint16_t fullNum = w * fullRow; // 完整的字节数
    for (uint8_t i = 0; i < w; i++)
    {
      OLED_SetBits_Fine(x + i, y + (fullRow * 8), data[fullNum + i], partBit, color);
    }
  }
  // 使用OLED_SetPixel实现
  // for (uint8_t i = 0; i < w; i++) {
  //   for (uint8_t j = 0; j < h; j++) {
  //     for (uint8_t k = 0; k < 8; k++) {
  //       if (j * 8 + k >= h) break; // 防止越界(不完整的字节
  //       OLED_SetPixel(x + i, y + j * 8 + k, !((data[i + j * w] >> k) & 0x01));
  //     }
  //   }
  // }
}

// ========================== 图形绘制函数 ==========================
/**
 * @brief 绘制一条线段
 * @param x1 起始点横坐标
 * @param y1 起始点纵坐标
 * @param x2 终止点横坐标
 * @param y2 终止点纵坐标
 * @param color 颜色
 * @note 此函数使用Bresenham算法绘制线段
 */
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, OLED_ColorMode color)
{
  static uint8_t temp = 0;
  if (x1 == x2)
  {
    if (y1 > y2)
    {
      temp = y1;
      y1 = y2;
      y2 = temp;
    }
    for (uint8_t y = y1; y <= y2; y++)
    {
      OLED_SetPixel(x1, y, color);
    }
  }
  else if (y1 == y2)
  {
    if (x1 > x2)
    {
      temp = x1;
      x1 = x2;
      x2 = temp;
    }
    for (uint8_t x = x1; x <= x2; x++)
    {
      OLED_SetPixel(x, y1, color);
    }
  }
  else
  {
    // Bresenham直线算法
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    int16_t ux = ((dx > 0) << 1) - 1;
    int16_t uy = ((dy > 0) << 1) - 1;
    int16_t x = x1, y = y1, eps = 0;
    dx = abs(dx);
    dy = abs(dy);
    if (dx > dy)
    {
      for (x = x1; x != x2; x += ux)
      {
        OLED_SetPixel(x, y, color);
        eps += dy;
        if ((eps << 1) >= dx)
        {
          y += uy;
          eps -= dx;
        }
      }
    }
    else
    {
      for (y = y1; y != y2; y += uy)
      {
        OLED_SetPixel(x, y, color);
        eps += dx;
        if ((eps << 1) >= dy)
        {
          x += ux;
          eps -= dy;
        }
      }
    }
  }
}

/**
 * @brief 绘制一个矩形
 * @param x 起始点横坐标
 * @param y 起始点纵坐标
 * @param w 矩形宽度
 * @param h 矩形高度
 * @param color 颜色
 */
void OLED_DrawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, OLED_ColorMode color)
{
  OLED_DrawLine(x, y, x + w, y, color);
  OLED_DrawLine(x, y + h, x + w, y + h, color);
  OLED_DrawLine(x, y, x, y + h, color);
  OLED_DrawLine(x + w, y, x + w, y + h, color);
}

/**
 * @brief 绘制一个填充矩形
 * @param x 起始点横坐标
 * @param y 起始点纵坐标
 * @param w 矩形宽度
 * @param h 矩形高度
 * @param color 颜色
 */
void OLED_DrawFilledRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, OLED_ColorMode color)
{
  for (uint8_t i = 0; i < h; i++)
  {
    OLED_DrawLine(x, y + i, x + w, y + i, color);
  }
}

/**
 * @brief 绘制一个三角形
 * @param x1 第一个点横坐标
 * @param y1 第一个点纵坐标
 * @param x2 第二个点横坐标
 * @param y2 第二个点纵坐标
 * @param x3 第三个点横坐标
 * @param y3 第三个点纵坐标
 * @param color 颜色
 */
void OLED_DrawTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, OLED_ColorMode color)
{
  OLED_DrawLine(x1, y1, x2, y2, color);
  OLED_DrawLine(x2, y2, x3, y3, color);
  OLED_DrawLine(x3, y3, x1, y1, color);
}

/**
 * @brief 绘制一个填充三角形
 * @param x1 第一个点横坐标
 * @param y1 第一个点纵坐标
 * @param x2 第二个点横坐标
 * @param y2 第二个点纵坐标
 * @param x3 第三个点横坐标
 * @param y3 第三个点纵坐标
 * @param color 颜色
 */
void OLED_DrawFilledTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, OLED_ColorMode color)
{
  uint8_t a = 0, b = 0, y = 0, last = 0;
  if (y1 > y2)
  {
    a = y2;
    b = y1;
  }
  else
  {
    a = y1;
    b = y2;
  }
  y = a;
  for (; y <= b; y++)
  {
    if (y <= y3)
    {
      OLED_DrawLine(x1 + (y - y1) * (x2 - x1) / (y2 - y1), y, x1 + (y - y1) * (x3 - x1) / (y3 - y1), y, color);
    }
    else
    {
      last = y - 1;
      break;
    }
  }
  for (; y <= b; y++)
  {
    OLED_DrawLine(x2 + (y - y2) * (x3 - x2) / (y3 - y2), y, x1 + (y - last) * (x3 - x1) / (y3 - last), y, color);
  }
}

/**
 * @brief 绘制一个圆
 * @param x 圆心横坐标
 * @param y 圆心纵坐标
 * @param r 圆半径
 * @param color 颜色
 * @note 此函数使用Bresenham算法绘制圆
 */
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r, OLED_ColorMode color)
{
  int16_t a = 0, b = r, di = 3 - (r << 1);
  while (a <= b)
  {
    OLED_SetPixel(x - b, y - a, color);
    OLED_SetPixel(x + b, y - a, color);
    OLED_SetPixel(x - a, y + b, color);
    OLED_SetPixel(x - b, y - a, color);
    OLED_SetPixel(x - a, y - b, color);
    OLED_SetPixel(x + b, y + a, color);
    OLED_SetPixel(x + a, y - b, color);
    OLED_SetPixel(x + a, y + b, color);
    OLED_SetPixel(x - b, y + a, color);
    a++;
    if (di < 0)
    {
      di += 4 * a + 6;
    }
    else
    {
      di += 10 + 4 * (a - b);
      b--;
    }
    OLED_SetPixel(x + a, y + b, color);
  }
}

/**
 * @brief 绘制一个填充圆
 * @param x 圆心横坐标
 * @param y 圆心纵坐标
 * @param r 圆半径
 * @param color 颜色
 * @note 此函数使用Bresenham算法绘制圆
 */
void OLED_DrawFilledCircle(uint8_t x, uint8_t y, uint8_t r, OLED_ColorMode color)
{
  int16_t a = 0, b = r, di = 3 - (r << 1);
  while (a <= b)
  {
    for (int16_t i = x - b; i <= x + b; i++)
    {
      OLED_SetPixel(i, y + a, color);
      OLED_SetPixel(i, y - a, color);
    }
    for (int16_t i = x - a; i <= x + a; i++)
    {
      OLED_SetPixel(i, y + b, color);
      OLED_SetPixel(i, y - b, color);
    }
    a++;
    if (di < 0)
    {
      di += 4 * a + 6;
    }
    else
    {
      di += 10 + 4 * (a - b);
      b--;
    }
  }
}

/**
 * @brief 绘制一个椭圆
 * @param x 椭圆中心横坐标
 * @param y 椭圆中心纵坐标
 * @param a 椭圆长轴
 * @param b 椭圆短轴
 */
void OLED_DrawEllipse(uint8_t x, uint8_t y, uint8_t a, uint8_t b, OLED_ColorMode color)
{
  int xpos = 0, ypos = b;
  int a2 = a * a, b2 = b * b;
  int d = b2 + a2 * (0.25 - b);
  while (a2 * ypos > b2 * xpos)
  {
    OLED_SetPixel(x + xpos, y + ypos, color);
    OLED_SetPixel(x - xpos, y + ypos, color);
    OLED_SetPixel(x + xpos, y - ypos, color);
    OLED_SetPixel(x - xpos, y - ypos, color);
    if (d < 0)
    {
      d = d + b2 * ((xpos << 1) + 3);
      xpos += 1;
    }
    else
    {
      d = d + b2 * ((xpos << 1) + 3) + a2 * (-(ypos << 1) + 2);
      xpos += 1, ypos -= 1;
    }
  }
  d = b2 * (xpos + 0.5) * (xpos + 0.5) + a2 * (ypos - 1) * (ypos - 1) - a2 * b2;
  while (ypos > 0)
  {
    OLED_SetPixel(x + xpos, y + ypos, color);
    OLED_SetPixel(x - xpos, y + ypos, color);
    OLED_SetPixel(x + xpos, y - ypos, color);
    OLED_SetPixel(x - xpos, y - ypos, color);
    if (d < 0)
    {
      d = d + b2 * ((xpos << 1) + 2) + a2 * (-(ypos << 1) + 3);
      xpos += 1, ypos -= 1;
    }
    else
    {
      d = d + a2 * (-(ypos << 1) + 3);
      ypos -= 1;
    }
  }
}

/**
 * @brief 绘制一张图片
 * @param x 起始点横坐标
 * @param y 起始点纵坐标
 * @param img 图片
 * @param color 颜色
 */
void OLED_DrawImage(uint8_t x, uint8_t y, const Image *img, OLED_ColorMode color)
{
  OLED_SetBlock(x, y, img->data, img->w, img->h, color);
}

// ================================ 文字绘制 ================================

/**
 * @brief 绘制一个ASCII字符
 * @param x 起始点横坐标
 * @param y 起始点纵坐标
 * @param ch 字符
 * @param font 字体
 * @param color 颜色
 */
void OLED_PrintASCIIChar(uint8_t x, uint8_t y, char ch, const ASCIIFont *font, OLED_ColorMode color)
{
  OLED_SetBlock(x, y, font->chars + (ch - ' ') * (((font->h + 7) / 8) * font->w), font->w, font->h, color);
}

/**
 * @brief 绘制一个ASCII字符串
 * @param x 起始点横坐标
 * @param y 起始点纵坐标
 * @param str 字符串
 * @param font 字体
 * @param color 颜色
 */
void OLED_PrintASCIIString(uint8_t x, uint8_t y, char *str, const ASCIIFont *font, OLED_ColorMode color)
{
  uint8_t x0 = x;
  while (*str)
  {
    OLED_PrintASCIIChar(x0, y, *str, font, color);
    x0 += font->w;
    str++;
  }
}

/**
 * @brief 获取UTF-8编码的字符长度
 */
uint8_t _OLED_GetUTF8Len(char *string)
{
  if ((string[0] & 0x80) == 0x00)
  {
    return 1;
  }
  else if ((string[0] & 0xE0) == 0xC0)
  {
    return 2;
  }
  else if ((string[0] & 0xF0) == 0xE0)
  {
    return 3;
  }
  else if ((string[0] & 0xF8) == 0xF0)
  {
    return 4;
  }
  return 0;
}

/**
 * @brief 绘制字符串
 * @param x 起始点横坐标
 * @param y 起始点纵坐标
 * @param str 字符串
 * @param font 字体
 * @param color 颜色
 *
 * @note 为保证字符串中的中文会被自动识别并绘制, 需:
 * 1. 编译器字符集设置为UTF-8
 * 2. 使用波特律动LED取模工具生成字模(https://led.baud-dance.com)
 */
/**
 * @brief 绘制字符串
 * @param x 起始点横坐标
 * @param y 起始点纵坐标
 * @param str 字符串
 * @param font 字体
 * @param color 颜色
 *
 * @note 为保证字符串中的中文会被自动识别并绘制, 需:
 * 1. 编译器字符集设置为UTF-8
 * 2. 使用波特律动LED取模工具生成字模(https://led.baud-dance.com)
 */
void OLED_PrintString(uint8_t x, uint8_t y, char *str, const Font *font, OLED_ColorMode color)
{
  uint16_t i = 0;                                       // 字符串索引
  uint8_t oneLen = (((font->h + 7) / 8) * font->w) + 4; // 一个字模占多少字节
  uint8_t found;                                        // 是否找到字模
  uint8_t utf8Len;                                      // UTF-8编码长度
  uint8_t *head;                                        // 字模头指针
  while (str[i])
  {
    found = 0;
    utf8Len = _OLED_GetUTF8Len(str + i);
    if (utf8Len == 0)
      break; // 有问题的UTF-8编码

    // 寻找字符  TODO 优化查找算法, 二分查找或者hash
    for (uint8_t j = 0; j < font->len; j++)
    {
      head = (uint8_t *)(font->chars) + (j * oneLen);
      if (memcmp(str + i, head, utf8Len) == 0)
      {
        OLED_SetBlock(x, y, head + 4, font->w, font->h, color);
        // 移动光标
        x += font->w;
        i += utf8Len;
        found = 1;
        break;
      }
    }

    // 若未找到字模,且为ASCII字符, 则缺省显示ASCII字符
    if (found == 0)
    {
      if (utf8Len == 1)
      {
        OLED_PrintASCIIChar(x, y, str[i], font->ascii, color);
        // 移动光标
        x += font->ascii->w;
        i += utf8Len;
      }
      else
      {
        OLED_PrintASCIIChar(x, y, ' ', font->ascii, color);
        x += font->ascii->w;
        i += utf8Len;
      }
    }
  }
}
