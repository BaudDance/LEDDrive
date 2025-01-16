/**
 * @file kk_knob.c
 * @brief 旋钮编码器驱动实现文件
 */

#include "kk_knob.h"

/**
 * @brief 编码器计数器初始值
 */
#define COUNTER_INIT_VALUE 65535/2

/**
 * @brief 按键消抖时间（毫秒）
 */
#define BTN_DEBOUNCE_TICKS 10

/**
 * @brief 按键状态枚举
 */
typedef enum {Pressed, Unpressed} BtnState;

/**
 * @brief 设置编码器计数器值
 * @param value 要设置的计数值
 */
void setCounter(int value){
	__HAL_TIM_SET_COUNTER(&htim1, value);
}

/**
 * @brief 获取编码器计数器值
 * @return 当前计数值
 */
uint32_t getCounter(){
	return __HAL_TIM_GET_COUNTER(&htim1);
}

/**
 * @brief 获取按键状态
 * @return 按键当前状态（按下/未按下）
 */
BtnState getBtnState(){
	return HAL_GPIO_ReadPin(KnobBtn_GPIO_Port, KnobBtn_Pin) == GPIO_PIN_RESET ? Pressed : Unpressed;
}

/**
 * @brief 获取系统时间
 * @return 系统时间戳
 */
uint32_t getTick(){
	return HAL_GetTick();
}

/** 旋钮正转回调函数指针 */
KnobCallback onForwardCallback = NULL;
/** 旋钮反转回调函数指针 */
KnobCallback onBackwardCallback = NULL;
/** 旋钮按下回调函数指针 */
KnobCallback onPressedCallback = NULL;

/**
 * @brief 设置旋钮正转回调函数
 * @param callback 回调函数指针
 */
void Knob_SetForwardCallback(KnobCallback callback){
	onForwardCallback = callback;
}

/**
 * @brief 设置旋钮反转回调函数
 * @param callback 回调函数指针
 */
void Knob_SetBackwardCallback(KnobCallback callback){
	onBackwardCallback = callback;
}

/**
 * @brief 设置旋钮按下回调函数
 * @param callback 回调函数指针
 */
void Knob_SetPressedCallback(KnobCallback callback){
	onPressedCallback = callback;
}

/**
 * @brief 旋钮初始化函数
 * @details 启动定时器编码器模式并初始化计数器
 */
void Knob_Init(){
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
	setCounter(COUNTER_INIT_VALUE);
}

/**
 * @brief 旋钮循环处理函数
 * @details 检测旋钮转动和按键状态，触发相应的回调函数
 */
void Knob_Loop(){
	uint32_t counter = getCounter();
	if (counter > COUNTER_INIT_VALUE){
		// 正转
		if (onForwardCallback != NULL){
			onForwardCallback();
		}
	}else if (counter < COUNTER_INIT_VALUE){
		// 反转
		if (onBackwardCallback != NULL){
			onBackwardCallback();
		}
	}
	setCounter(COUNTER_INIT_VALUE);

	BtnState btnState = getBtnState();
	static uint8_t callbackState = 0; // 0:未调用 1:已调用
	static uint32_t pressedTime = 0;

	if (btnState == Pressed){
		if (pressedTime == 0){
			pressedTime = getTick();
		}else if (callbackState == 0 && getTick() - pressedTime > BTN_DEBOUNCE_TICKS){
			// 按键消抖完成 且尚未执行回调函数
			if (onPressedCallback != NULL){
				onPressedCallback();
			}
			callbackState = 1;
		}
	}else {
		// 按键松开
		pressedTime = 0;
		callbackState = 0;
	}
}
