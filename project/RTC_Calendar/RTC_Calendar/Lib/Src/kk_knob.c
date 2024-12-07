#include "kk_knob.h"

// 计数器最大值
#define COUNT_MAX 65535
// 按键消抖滴答数
#define BTN_DEBOUNCE_TICKS 10

typedef enum { Pressed, Unpressed } BtnState;

/**
 * @brief 获取定时器计数器的值
 * @retval 计数器的值
 * @note 可通过本函数进行代码的移植
 */
uint32_t _GetCounterValue() {
  return __HAL_TIM_GetCounter(&htim1);
}

/**
 * @brief 读取旋钮按键状态
 * @retval 按键状态
 * @note 可通过本函数进行代码的移植
 */
BtnState _ReadBtn() {
  return HAL_GPIO_ReadPin(KnobBtn_GPIO_Port, KnobBtn_Pin) == GPIO_PIN_RESET ? Pressed : Unpressed;
}

/**
 * @brief 获取系统滴答计数器的值
 * @retval 滴答计数器的值
 * @note 可通过本函数进行代码的移植
 */
uint32_t _GetSysTick() {
  return HAL_GetTick();
}

void Knob_Init() {
  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
}

// 旋钮回调函数
KnobCallback onForwardCallback = NULL;
KnobCallback onBackwardCallback = NULL;
KnobCallback onBtnPressCallback = NULL;

/**
 * @brief 设置旋钮向前旋转时的回调函数
 * @param callback 回调函数
 */
void Knob_SetOnForwardCallback(KnobCallback callback) {
  onForwardCallback = callback;
}

/**
 * @brief 设置旋钮向后旋转时的回调函数
 * @param callback 回调函数
 */
void Knob_SetOnBackwardCallback(KnobCallback callback) {
  onBackwardCallback = callback;
}

/**
 * @brief 设置旋钮按键按下时的回调函数
 * @param callback 回调函数
 */
void Knob_SetOnBtnPressCallback(KnobCallback callback) {
  onBtnPressCallback = callback;
}

void Knob_TaskLoop() {
  // 判断旋钮
  static uint32_t lastCounterValue = 0;
  uint32_t counterValue = _GetCounterValue();
  if (counterValue != lastCounterValue) {
    if (((counterValue > lastCounterValue) && (counterValue - lastCounterValue < COUNT_MAX / 2)) || (counterValue < 10 && lastCounterValue > COUNT_MAX - 10)) {
      if (onForwardCallback != NULL) {
        onForwardCallback();
      }
    } else {
      if (onBackwardCallback != NULL) {
        onBackwardCallback();
      }
    }
    lastCounterValue = counterValue;
  }
  // 判断按键
  static BtnState lastBtnState = Unpressed;
  static uint32_t lastBtnPressTime = 0;
  BtnState btnState = _ReadBtn();
  if (btnState == Unpressed) {
    // 按键未按下
    lastBtnPressTime = 0;
    lastBtnState = Unpressed;
  } else if (lastBtnPressTime == 0) {
    // 按键刚刚按下
    lastBtnPressTime = _GetSysTick();
  } else if (lastBtnState == Unpressed && _GetSysTick() - lastBtnPressTime > BTN_DEBOUNCE_TICKS) {
    // 按键按下且消抖完成 但尚未执行回调
    if (onBtnPressCallback != NULL) {
      onBtnPressCallback();
    }
    lastBtnState = Pressed;
  }
}
