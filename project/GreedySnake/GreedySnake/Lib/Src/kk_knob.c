#include "kk_knob.h"

#define COUNTER_INIT_VALUE 65535/2
#define BTN_DEBOUNCE_TICKS 10

typedef enum {Pressed, Unpressed} BtnState;

void setCounter(int value){
	__HAL_TIM_SET_COUNTER(&htim1, value);
}

uint32_t getCounter(){
	return __HAL_TIM_GET_COUNTER(&htim1);
}

BtnState getBtnState(){
	return HAL_GPIO_ReadPin(KnobBtn_GPIO_Port, KnobBtn_Pin) == GPIO_PIN_RESET ? Pressed : Unpressed;
}

uint32_t getTick(){
	return HAL_GetTick();
}

KnobCallback onForwardCallback = NULL;
KnobCallback onBackwardCallback = NULL;
KnobCallback onPressedCallback = NULL;

void Knob_SetForwardCallback(KnobCallback callback){
	onForwardCallback = callback;
}

void Knob_SetBackwardCallback(KnobCallback callback){
	onBackwardCallback = callback;
}

void Knob_SetPressedCallback(KnobCallback callback){
	onPressedCallback = callback;
}

void Knob_Init(){
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
	setCounter(COUNTER_INIT_VALUE);
}

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






















