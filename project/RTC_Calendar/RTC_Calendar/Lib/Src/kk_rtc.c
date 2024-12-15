#include "kk_rtc.h"

// RTC已经被初始化的值 记录在RTC_BKP_DR1中
#define RTC_INIT_FLAG 0x2333

/**
 * @brief  进入RTC初始化模式
 * @param  hrtc  指向包含RTC配置信息的RTC_HandleTypeDef结构体的指针
 * @retval HAL status
 */
static HAL_StatusTypeDef RTC_EnterInitMode(RTC_HandleTypeDef *hrtc)
{
  uint32_t tickstart = 0U;

  tickstart = HAL_GetTick();
  /* 等待RTC处于INIT状态，如果到达Time out 则退出 */
  while ((hrtc->Instance->CRL & RTC_CRL_RTOFF) == (uint32_t)RESET)
  {
    if ((HAL_GetTick() - tickstart) >  RTC_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* 禁用RTC寄存器的写保护 */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);


  return HAL_OK;
}

/**
 * @brief  退出RTC初始化模式
 * @param  hrtc   指向包含RTC配置信息的RTC_HandleTypeDef结构体的指针
 * @retval HAL status
 */
static HAL_StatusTypeDef RTC_ExitInitMode(RTC_HandleTypeDef *hrtc)
{
  uint32_t tickstart = 0U;

  /* 禁用RTC寄存器的写保护。 */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);

  tickstart = HAL_GetTick();
  /* 等到RTC处于INIT状态，如果到达Time out 则退出 */
  while ((hrtc->Instance->CRL & RTC_CRL_RTOFF) == (uint32_t)RESET)
  {
    if ((HAL_GetTick() - tickstart) >  RTC_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  return HAL_OK;
}

/**
 * @brief  写入RTC_CNT寄存器中的时间计数器。
 * @param  hrtc   指向包含RTC配置信息的RTC_HandleTypeDef结构体的指针。
 * @param  TimeCounter: 写入RTC_CNT寄存器的计数器
 * @retval HAL status
 */
static HAL_StatusTypeDef RTC_WriteTimeCounter(RTC_HandleTypeDef *hrtc, uint32_t TimeCounter)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* 进入RTC初始化模式 */
  if (RTC_EnterInitMode(hrtc) != HAL_OK)
  {
    status = HAL_ERROR;
  }
  else
  {
	/* 设置RTC计数器高位寄存器 */
    WRITE_REG(hrtc->Instance->CNTH, (TimeCounter >> 16U));
    /* 设置RTC计数器低位寄存器 */
    WRITE_REG(hrtc->Instance->CNTL, (TimeCounter & RTC_CNTL_RTC_CNT));

    /* 退出RTC初始化模式 */
    if (RTC_ExitInitMode(hrtc) != HAL_OK)
    {
      status = HAL_ERROR;
    }
  }

  return status;
}


/**
 * @brief  读取RTC_CNT寄存器中的时间计数器。
 * @param  hrtc   指向包含RTC配置信息的RTC_HandleTypeDef结构体的指针。
 * @retval 时间计数器
 */
static uint32_t RTC_ReadTimeCounter(RTC_HandleTypeDef *hrtc)
{
  uint16_t high1 = 0U, high2 = 0U, low = 0U;
  uint32_t timecounter = 0U;

  high1 = READ_REG(hrtc->Instance->CNTH & RTC_CNTH_RTC_CNT);
  low   = READ_REG(hrtc->Instance->CNTL & RTC_CNTL_RTC_CNT);
  high2 = READ_REG(hrtc->Instance->CNTH & RTC_CNTH_RTC_CNT);

  if (high1 != high2)
  {
	/* 当读取CNTL和CNTH寄存器期间计数器溢出时, 重新读取CNTL寄存器然后返回计数器值 */
    timecounter = (((uint32_t) high2 << 16U) | READ_REG(hrtc->Instance->CNTL & RTC_CNTL_RTC_CNT));
  }
  else
  {
	/* 当读取CNTL和CNTH寄存器期间没有计数器溢出, 计数器值等于第一次读取的CNTL和CNTH值 */
    timecounter = (((uint32_t) high1 << 16U) | low);
  }

  return timecounter;
}

/**
 * @brief 设置RTC时间
 * @param time 时间
 * @retval HAL status
 */
HAL_StatusTypeDef KK_RTC_SetTime(struct tm *time){
	uint32_t unixTime = mktime(time);
	return RTC_WriteTimeCounter(&hrtc, unixTime);
}

/**
 * @brief 获取RTC时间
 * @retval 时间
 */
struct tm *KK_RTC_GetTime() {
  time_t unixTime = RTC_ReadTimeCounter(&hrtc);
  return gmtime(&unixTime);
}

void KK_RTC_Init(){
	uint32_t initFlag = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
	if(initFlag == RTC_INIT_FLAG) return;
	if (HAL_RTC_Init(&hrtc) != HAL_OK){
		Error_Handler();
	}
	struct tm time = {
		  .tm_year = 2025 - 1900,
		  .tm_mon = 1 - 1,
		  .tm_mday = 1,
		  .tm_hour = 23,
		  .tm_min = 59,
		  .tm_sec = 55,
	};
	KK_RTC_SetTime(&time);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, RTC_INIT_FLAG);

}


