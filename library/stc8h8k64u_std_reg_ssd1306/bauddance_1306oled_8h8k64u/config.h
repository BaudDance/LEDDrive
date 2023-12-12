/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/
#ifndef		__CONFIG_H
#define		__CONFIG_H

/*********************************************************/

//#define MAIN_Fosc		22118400L	//定义主时钟
//#define MAIN_Fosc		12000000L	//定义主时钟
//#define MAIN_Fosc		11059200L	//定义主时钟
//#define MAIN_Fosc		 5529600L	//定义主时钟
#define MAIN_Fosc		24000000L	//定义主时钟


#define STC8Hxx		//STC8H系列
//#define STC8Gxx		//STC8G系列

//#ifndef EAXSFR()
//#define EAXSFR() P_SW2 |= 0x80
//#endif
//#define EAXRAM() AUXR &= ~0x02

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long int u32;

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long int uint32_t;
typedef signed char int8_t;
typedef signed int int16_t;
typedef signed long int int32_t;
/*********************************************************/

#include "STC8H.H"
#include "intrins.h"
//#include "main.h"


#endif
