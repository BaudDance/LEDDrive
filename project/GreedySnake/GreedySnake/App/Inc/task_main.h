#ifndef __TASK_MAIN_H__
#define __TASK_MAIN_H__

#include "oled.h"
#include "i2c.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "kk_knob.h"

void MainTaskInit();
void MainTask();

#endif /* __TASK_MAIN_H__ */