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
#include	"STC8G_H_I2C.h"

u8 xdata I2C_Buffer[I2C_BUF_LENTH];

//========================================================================
// 函数: void I2C_Init(I2C_InitTypeDef *I2Cx)
// 描述: I2C初始化程序.
// 参数: I2Cx: 结构参数,请参考I2C.h里的定义.
// 返回: none.
// 版本: V1.0, 2012-11-22
//========================================================================
void I2C_Init(I2C_InitTypeDef *I2Cx)
{
	if(I2Cx->I2C_Mode == I2C_Mode_Master)
	{
		I2C_Master();			//设为主机	
		I2CMSST = 0x00;		//清除I2C主机状态寄存器
		I2C_SetSpeed(I2Cx->I2C_Speed);
		if(I2Cx->I2C_MS_WDTA == ENABLE)		I2C_WDTA_EN();	//使能自动发送
		else									I2C_WDTA_DIS();	//禁止自动发送
	}
	else
	{
		I2C_Slave();	//设为从机
		I2CSLST = 0x00;		//清除I2C从机状态寄存器
		I2C_Address(I2Cx->I2C_SL_ADR);
		if(I2Cx->I2C_SL_MA == ENABLE)		I2C_MATCH_EN();	//从机地址比较功能，只接受相匹配地址
		else									I2C_MATCH_DIS();	//禁止从机地址比较功能，接受所有设备地址
	}
	
	I2C_Function(I2Cx->I2C_Enable);
}

//========================================================================
// 函数: u8	Get_MSBusy_Status (void)
// 描述: 获取主机忙碌状态.
// 参数: none.
// 返回: 主机忙碌状态.
// 版本: V1.0, 2012-11-22
//========================================================================
u8 Get_MSBusy_Status(void)
{
	return (I2CMSST & 0x80);
}

//========================================================================
// 函数: void	Wait (void)
// 描述: 等待主机模式I2C控制器执行完成I2CMSCR.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2012-11-22
//========================================================================
void Wait()
{
	while (!(I2CMSST & 0x40));
	I2CMSST &= ~0x40;
}

//========================================================================
// 函数: void Start (void)
// 描述: I2C总线起始函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-15
//========================================================================
void Start()
{
	I2CMSCR = 0x01;                         //发送START命令
	Wait();
}

//========================================================================
// 函数: void SendData (char dat)
// 描述: I2C发送一个字节数据函数.
// 参数: 发送的数据.
// 返回: none.
// 版本: V1.0, 2020-09-15
//========================================================================
void SendData(char dat)
{
	I2CTXD = dat;                           //写数据到数据缓冲区
	I2CMSCR = 0x02;                         //发送SEND命令
	Wait();
}

//========================================================================
// 函数: void RecvACK (void)
// 描述: I2C获取ACK函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-15
//========================================================================
void RecvACK()
{
	I2CMSCR = 0x03;                         //发送读ACK命令
	Wait();
}

//========================================================================
// 函数: char RecvData (void)
// 描述: I2C读取一个字节数据函数.
// 参数: none.
// 返回: 读取数据.
// 版本: V1.0, 2020-09-15
//========================================================================
char RecvData()
{
	I2CMSCR = 0x04;                         //发送RECV命令
	Wait();
	return I2CRXD;
}

//========================================================================
// 函数: void SendACK (void)
// 描述: I2C发送ACK函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-15
//========================================================================
void SendACK()
{
	I2CMSST = 0x00;                         //设置ACK信号
	I2CMSCR = 0x05;                         //发送ACK命令
	Wait();
}

//========================================================================
// 函数: void SendNAK (void)
// 描述: I2C发送NAK函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-15
//========================================================================
void SendNAK()
{
	I2CMSST = 0x01;                         //设置NAK信号
	I2CMSCR = 0x05;                         //发送ACK命令
	Wait();
}

//========================================================================
// 函数: void Stop (void)
// 描述: I2C总线停止函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-15
//========================================================================
void Stop()
{
	I2CMSCR = 0x06;                         //发送STOP命令
	Wait();
}

//========================================================================
// 函数: void SendCmdData (u8 cmd, u8 dat)
// 描述: I2C发送一个字节数据函数.
// 参数: 命令/数据.
// 返回: none.
// 版本: V1.0, 2020-09-15
//========================================================================
void SendCmdData(u8 cmd, u8 dat)
{
	I2CTXD = dat;                           //写数据到数据缓冲区
	I2CMSCR = cmd;                          //设置命令
	Wait();
}

//========================================================================
// 函数: void I2C_WriteNbyte(u8 dev_addr, u8 mem_addr, u8 *p, u8 number)
// 描述: I2C写入数据函数.
// 参数: dev_addr: 设备地址, mem_addr: 存储地址, *p写入数据存储位置, number写入数据个数.
// 返回: none.
// 版本: V1.0, 2020-09-15
//========================================================================
void I2C_WriteNbyte(u8 dev_addr, u8 mem_addr, u8 *p, u8 number)  /*  DeviceAddress,WordAddress,First Data Address,Byte lenth   */
{
	Start();                                //发送起始命令
	SendData(dev_addr);                     //发送设备地址+写命令
	RecvACK();
	SendData(mem_addr);                     //发送存储地址
	RecvACK();
	do
	{
		SendData(*p++);
		RecvACK();
	}
	while(--number);
	Stop();                                 //发送停止命令
}

void I2C_WriteNbyte_WithoutmemaddrWrite(u8 dev_addr, u8 *p, u8 number)  /*  DeviceAddress,WordAddress,First Data Address,Byte lenth   */
{
	Start();                                //发送起始命令
	SendData(dev_addr);                     //发送设备地址+写命令
	RecvACK();
//	SendData(mem_addr);                     //发送存储地址
//	RecvACK();
	do
	{
		SendData(*p++);
		RecvACK();
	}
	while(--number);
	Stop();                                 //发送停止命令
}

//========================================================================
// 函数: void I2C_ReadNbyte(u8 dev_addr, u8 mem_addr, u8 *p, u8 number)
// 描述: I2C读取数据函数.
// 参数: dev_addr: 设备地址, mem_addr: 存储地址, *p读取数据存储位置, number读取数据个数.
// 返回: none.
// 版本: V1.0, 2020-09-15
//========================================================================
void I2C_ReadNbyte(u8 dev_addr, u8 mem_addr, u8 *p, u8 number)   /*  DeviceAddress,WordAddress,First Data Address,Byte lenth   */
{
	Start();                                //发送起始命令
	SendData(dev_addr);                     //发送设备地址+写命令
	RecvACK();
	SendData(mem_addr);                     //发送存储地址
	RecvACK();
	Start();                                //发送起始命令
	SendData(dev_addr|1);                   //发送设备地址+读命令
	RecvACK();
	do
	{
		*p = RecvData();
		p++;
		if(number != 1) SendACK();          //send ACK
	}
	while(--number);
	SendNAK();                              //send no ACK	
	Stop();                                 //发送停止命令
}

void I2C_ReadNbyte_WithoutmemaddrWrite(u8 dev_addr, u8 *p, u8 number)   /*  DeviceAddress,First Data Address,Byte lenth   */
{
//	Start();                                //发送起始命令
//	SendData(dev_addr);                     //发送设备地址+写命令
//	RecvACK();
//	SendData(mem_addr);                     //发送存储地址
//	RecvACK();
	Start();                                //发送起始命令
	SendData(dev_addr|1);                   //发送设备地址+读命令
	RecvACK();
	do
	{
		*p = RecvData();
		p++;
		if(number != 1) SendACK();          //send ACK
	}
	while(--number);
	SendNAK();                              //send no ACK	
	Stop();                                 //发送停止命令
}