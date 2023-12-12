#ifndef __FONT_H
#define __FONT_H
//#include "stdint.h"
#include "string.h"
#include "main.h"
#include "config.h"

typedef struct ASCIIFont {
  uint8_t h;
  uint8_t w;
  uint8_t *chars;
} ASCIIFont;

//typedef struct {
//  uint8_t h;
//  uint8_t w;
//  uint8_t *chars;
//} ASCIIFont;

/**
 * @brief 字体结构体
 * @note  字库前4字节存储utf8编码 剩余字节存储字模数据
 * @note 字库数据可以使用波特律动LED取模助手生成(https://led.baud-dance.com)
 */
typedef struct Font {
  uint8_t h;              // 字高度
  uint8_t w;              // 字宽度
  const uint8_t *chars;   // 字库 字库前4字节存储utf8编码 剩余字节存储字模数据
  uint8_t len;            // 字库长度 超过256则请改为uint16_t
  const ASCIIFont *ascii; // 缺省ASCII字体 当字库中没有对应字符且需要显示ASCII字符时使用
} Font;

//typedef struct {
//  uint8_t h;              // 字高度
//  uint8_t w;              // 字宽度
//  const uint8_t *chars;   // 字库 字库前4字节存储utf8编码 剩余字节存储字模数据
//  uint8_t len;            // 字库长度 超过256则请改为uint16_t
//  const ASCIIFont *ascii; // 缺省ASCII字体 当字库中没有对应字符且需要显示ASCII字符时使用
//} Font;

/**
 * @brief 图片结构体
 * @note  图片数据可以使用波特律动LED取模助手生成(https://led.baud-dance.com)
 */
typedef struct Image {
  uint8_t w;           // 图片宽度
  uint8_t h;           // 图片高度
  const uint8_t *data_8; // 图片数据
}Image;

//typedef struct {
//  uint8_t w;           // 图片宽度
//  uint8_t h;           // 图片高度
//  const uint8_t *data_8; // 图片数据
//} Image;

extern const struct ASCIIFont code afont8x6;
extern const struct ASCIIFont code afont12x6;
extern const struct ASCIIFont code afont16x8;
extern const struct ASCIIFont code afont24x12;
extern const struct Font code font16x16;
extern const struct Image code bilibiliImg;

#endif // __FONT_H
