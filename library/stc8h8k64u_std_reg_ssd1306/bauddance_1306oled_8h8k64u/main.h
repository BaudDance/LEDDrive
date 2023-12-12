#ifndef __MAIN_H_
#define __MAIN_H_

#include "config.h"

#ifndef ENABLE
#define ENABLE 1
#endif

#ifndef DISABLE
#define DISABLE 0
#endif

#ifndef I2C_Mode_Master
#define I2C_Mode_Master 1
#endif

#ifndef I2C_Mode_Slave
#define I2C_Mode_Slave 0
#endif

void Delay_ms(uint8_t time_ms);
void GPIO_Init_OD();

#endif
