/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/
/*
*与STC官方库相比，新增
*void I2C_WriteNbyte_WithoutmemaddrWrite(u8 dev_addr, u8 *p, u8 number)
*与
*void I2C_ReadNbyte_WithoutmemaddrWrite(u8 dev_addr, u8 *p, u8 number)
*函数
*
*2023/12/11 FQL
*/
#ifndef	__STC8G_H_I2C_H
#define	__STC8G_H_I2C_H

#include "config.h"
#include "main.h"

//========================================================================
//                               I2C设置
//========================================================================

#define		I2C_Function(n)	(n==0?(I2CCFG &= ~0x80):(I2CCFG |= 0x80))	//0：禁止 I2C 功能；1：使能 I2C 功能
#define		I2C_ENABLE()	I2CCFG |= 0x80		/* 使能 I2C 功能 */
#define		I2C_DISABLE()	I2CCFG &= ~0x80		/* 禁止 I2C 功能 */
#define		I2C_Master()	I2CCFG |=  0x40		/* 1: 设为主机	*/
#define		I2C_Slave()		I2CCFG &= ~0x40		/* 0: 设为从机	*/
#define		I2C_SetSpeed(n)	I2CCFG = (I2CCFG & ~0x3f) | (n & 0x3f)	/* 总线速度=Fosc/2/(Speed*2+4) */

#define		I2C_WDTA_EN()		I2CMSAUX |= 0x01		/* 使能自动发送 */
#define		I2C_WDTA_DIS()	I2CMSAUX &= ~0x01		/* 禁止自动发送 */

#define		I2C_ESTAI_EN(n)		I2CSLCR = (I2CSLCR & ~0x40) | (n << 6)		/* 使能从机接收START信号中断 */
#define		I2C_ERXI_EN(n)		I2CSLCR = (I2CSLCR & ~0x20) | (n << 5)		/* 使能从机接收1字节数据中断 */
#define		I2C_ETXI_EN(n)		I2CSLCR = (I2CSLCR & ~0x10) | (n << 4)		/* 使能从机发送1字节数据中断 */
#define		I2C_ESTOI_EN(n)		I2CSLCR = (I2CSLCR & ~0x08) | (n << 3)		/* 使能从机接收STOP信号中断 */
#define		I2C_SLRET()				I2CSLCR |= 0x01			/* 复位从机模式 */

#define		I2C_Address(n)	I2CSLADR = (I2CSLADR & 0x01) | (n << 1)	/* 从机地址 */
#define		I2C_MATCH_EN()	I2CSLADR &= ~0x01	/* 使能从机地址比较功能，只接受相匹配地址 */
#define		I2C_MATCH_DIS()	I2CSLADR |= 0x01	/* 禁止从机地址比较功能，接受所有设备地址 */

//========================================================================
//                              定义声明
//========================================================================

#define DEV_ADDR    0xA0    //从机设备写地址

#define	I2C_BUF_LENTH	8

#define		I2C_ESTAI					0x40		/* 从机接收START信号中断 */
#define		I2C_ERXI					0x20		/* 从机接收1字节数据中断 */
#define		I2C_ETXI					0x10		/* 从机发送1字节数据中断 */
#define		I2C_ESTOI					0x08		/* 从机接收STOP信号中断 */

typedef struct
{
	u8	I2C_Speed;				//总线速度=Fosc/2/(Speed*2+4),      0~63
	u8	I2C_Enable;				//I2C功能使能,   ENABLE, DISABLE
	u8	I2C_Mode;					//主从模式选择,  I2C_Mode_Master,I2C_Mode_Slave
	u8	I2C_MS_WDTA;				//主机使能自动发送,  ENABLE, DISABLE

	u8	I2C_SL_ADR;				//从机设备地址,  0~127
	u8	I2C_SL_MA;				//从机设备地址比较使能,  ENABLE, DISABLE
} I2C_InitTypeDef;

typedef struct
{
	u8	isma;				//MEMORY ADDRESS 接收判断标志
	u8	isda;				//DEVICE ADDRESS 接收判断标志
	u8	addr;				//ADDRESS 缓存
} I2C_IsrTypeDef;


extern u8 xdata I2C_Buffer[I2C_BUF_LENTH];
extern bit DisplayFlag;

void I2C_Init(I2C_InitTypeDef *I2Cx);
void I2C_WriteNbyte(u8 dev_addr, u8 mem_addr, u8 *p, u8 number);
void I2C_WriteNbyte_WithoutmemaddrWrite(u8 dev_addr, u8 *p, u8 number);
void I2C_ReadNbyte(u8 dev_addr, u8 mem_addr, u8 *p, u8 number);
void I2C_ReadNbyte_WithoutmemaddrWrite(u8 dev_addr, u8 *p, u8 number);
u8 Get_MSBusy_Status(void);
void SendCmdData(u8 cmd, u8 dat);

#endif

