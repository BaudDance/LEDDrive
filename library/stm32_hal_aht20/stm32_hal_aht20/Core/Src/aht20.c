/**
 * @file aht20.c
 * @brief 波特律动AHT20(DHT20)驱动
 * @anchor 波特律动(keysking 博哥在学习)
 * @version 1.0
 * @date 2023-08-21
 * @license MIT License
 *
 * @attention
 * 本驱动库基于波特律动·keysking的STM32教程学习套件进行开发
 * 在其他硬件平台上使用可能需要进行简单地移植: 修改Send与Reveive函数
 *
 * @note
 * 使用流程:
 * 1. STM32初始化IIC完成后调用AHT20_Init()初始化AHT20.
 * 2. 调用AHT20_Measure()可以进行一次测量
 * 3. 调用AHT20_Temperature()与AHT20_Humidity()分别可以获取上次测量时的温度与湿度数据
 */
#include "aht20.h"
#include "i2c.h"

// AHT20 IIC器件地址
#define AHT20_ADDRESS 0x70

// 温湿度数据暂存变量
float Temperature;
float Humidity;

// ========================== 底层通信函数 ==========================

/**
 * @brief 向AHT20发送数据的函数
 * @param data 发送的数据
 * @param len 发送的数据长度
 * @return void
 * @note 此函数是移植本驱动时的重要函数 将本驱动库移植到其他平台时应根据实际情况修改此函数
 */
void AHT20_Send(uint8_t *data, uint8_t len) {
  HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, data, len, HAL_MAX_DELAY);
}

/**
 * @brief 从AHT20接收数据的函数
 * @param data 接收数据的缓冲区
 * @param len 接收数据的长度
 * @return void
 * @note 此函数是移植本驱动时的重要函数 将本驱动库移植到其他平台时应根据实际情况修改此函数
 */
void AHT20_Receive(uint8_t *data, uint8_t len) {
  HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, data, len, HAL_MAX_DELAY);
}

/**
 * @brief AHT20初始化函数
 */
void AHT20_Init() {
  uint8_t readBuffer;
  HAL_Delay(40);
  AHT20_Receive(&readBuffer, 1);
  if ((readBuffer & 0x08) == 0x00) {
    uint8_t sendBuffer[3] = {0xBE, 0x08, 0x00};
    AHT20_Send(sendBuffer, 3);
  }
}

/**
 * @brief AHT20测量函数
 * @note 测量完成后可以通过AHT20_Temperature()与AHT20_Humidity()获取温度与湿度数据
 */
void AHT20_Measure() {
  uint8_t sendBuffer[3] = {0xAC, 0x33, 0x00};
  uint8_t readBuffer[6];
  AHT20_Send(sendBuffer, 3);
  HAL_Delay(75);
  AHT20_Receive(readBuffer, 6);

  if ((readBuffer[0] & 0x80) == 0x00) {
    float humi, temp;
    #ifdef __CMSIS_GCC_H //只快一点点，但是看起来可能更直观一些：__REV()是硬实现的大小端序转换，在cmsis_gcc.h中定义
    humi = (__REV(*(uint32_t*)readBuffer) & 0x00fffff0) >> 4;
    temp = __REV(*(uint32_t*)(readBuffer+2)) & 0x000fffff;
    #else
    humi = ((uint32_t)readBuffer[3] >> 4) + ((uint32_t)readBuffer[2] << 4) + ((uint32_t)readBuffer[1] << 12);
    temp = (((uint32_t)readBuffer[3] & 0x0F) << 16) + (((uint32_t)readBuffer[4]) << 8) + (uint32_t)readBuffer[5];
    #endif
    Humidity = humi * 100 / (1 << 20);
    Temperature = temp * 200 / (1 << 20) - 50;
  }
}

/**
 * @brief 获取上次测量时的温度数据
 */
float AHT20_Temperature() {
  return Temperature;
}

/**
 * @brief 获取上次测量时的湿度数据
 */
float AHT20_Humidity() {
  return Humidity;
}
