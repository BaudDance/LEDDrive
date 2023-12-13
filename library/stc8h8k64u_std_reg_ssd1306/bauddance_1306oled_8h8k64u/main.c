/*波特律动OLED演示函数 8051 Version
 *适用于STC15,STC8等具有硬件I2C, flash >= 16kB, sram >= 2kB的型号
 *测试MCU：STC8H8K64U-TSSOP20，默认情况下SDA位于P1.4，SCL位于P1.5
 *Note:请勿随意改变函数、变量定义/声明的位置
 *Note:结构体变量定义/声明时需要struct关键字（可能是编译器的BUG）
 *Note:code关键字用于确保常量位于flash中，请勿删除
 *Note:编译选项Memory Model选择Large:variables in XDATA
 *MAIN_Fosc		24000000L
 *其他下载选项保持默认
 *2023/12/11 FQL
*/
#include "main.h"
#include "stdio.h"
#include "oled.h"
#include "STC8G_H_I2C.h"

void main()
{
	uint8_t i;
	I2C_InitTypeDef I2C0;
	I2C0.I2C_Speed = 13;//总线速度=Fosc/2/(Speed*2+4),      0~63, 400kHz
	I2C0.I2C_Enable = ENABLE;
	I2C0.I2C_Mode = I2C_Mode_Master;
	I2C0.I2C_MS_WDTA = DISABLE;//主机不使能自动发送

	EAXSFR();
	EAXRAM();

	GPIO_Init_OD();
	I2C_Init(&I2C0);
	Delay_ms(100);
	OLED_Init(); 

  while (1)
	{
		for (i = 0; i < 256; i++)
    {
      OLED_NewFrame();
      OLED_DrawImage((128 - (bilibiliImg.w)) / 2, 0, &bilibiliImg, OLED_COLOR_NORMAL);
      OLED_PrintString(128 - i, 64 - 16, "波特律动hello", &font16x16, OLED_COLOR_NORMAL);
      OLED_ShowFrame();
    }
	}
}

void Delay_ms(uint8_t time_ms)	//@24.000MHz
{
	uint8_t k;
	for(k = 0;k < time_ms;k++)
	{
		uint8_t data i, j;

		_nop_();
		i = 32;
		j = 40;
		do
		{
			while (--j);
		} while (--i);
	}
	
}

void GPIO_Init_OD()
{
//	P3M0 |= 0x0c;
//	P3M1 |= 0x0c;//P3.2与P3.3设为开漏输出
	P1M0 |= 0x30;
	P1M1 |= 0x30;//P1.5与P1.4设为开漏输出

	
	P_SW2 &= ~0x30;//I2C_S = 00, P1.5(P3.2) = SCL, P1.4(P3.3) = SDA
}


