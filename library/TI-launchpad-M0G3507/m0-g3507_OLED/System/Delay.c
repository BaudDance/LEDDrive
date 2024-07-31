#include "ti_msp_dl_config.h"

/* 
*函数名称：void delay_ms(uint32_t delay_ms_x)
*函数输入：uint32_t delay_ms_x 延时函数一次性延时1ms
*函数输出：无
*函数注释：可以随着M0时钟频率的变化变换，总之就是会稳定的延时一毫秒
*/ 
void Delay_ms(uint32_t delay_ms_x)
{
    while(delay_ms_x--)
    {
        DL_Common_delayCycles(CPUCLK_FREQ/1000);
    }
}