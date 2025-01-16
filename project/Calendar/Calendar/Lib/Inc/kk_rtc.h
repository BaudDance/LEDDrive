#ifndef INC_KK_RTC_H_
#define INC_KK_RTC_H_
#include "stm32f1xx_hal.h"
#include "rtc.h"
#include "time.h"

HAL_StatusTypeDef KK_RTC_SetTime(struct tm *time);
struct tm *KK_RTC_GetTime();
void KK_RTC_Init();

#endif /* INC_KK_RTC_H_ */
