/**
 * @file task_main.c
 * @brief 主任务文件，实现日历显示和时间设置功能
 */

#include "task_main.h"

/**
 * @brief 光标闪烁间隔时间(毫秒)
 */
#define CURSOR_FLASH_INTERVAL 500

/**
 * @brief 星期几的字符串数组
 */
char weeks[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/**
 * @brief 日历状态枚举
 */
typedef enum {
	CalendarState_Normal, // 普通显示模式
	CalendarState_Setting // 时间设置模式
} CalendarState;

/**
 * @brief 设置状态枚举，表示当前正在设置的时间项
 */
typedef enum {Year = 0, Month, Day, Hour, Minute, Second} SettingState;

/**
 * @brief 光标位置结构体
 */
typedef struct {uint8_t x1; uint8_t y1; uint8_t x2; uint8_t y2;} CursorPosition;

/**
 * @brief 各时间项的光标位置数组
 */
CursorPosition cursorPositions[6] = {
	{24 + 0 * 8, 17, 24 + 4 * 8, 17},   // Year
	{24 + 5 * 8, 17, 24 + 7 * 8, 17},   // Month
	{24 + 8 * 8, 17, 24 + 10 * 8, 17},  // Day
	{16 + 0 * 12, 45, 16 + 2 * 12, 45}, // Hour
	{16 + 3 * 12, 45, 16 + 5 * 12, 45}, // Minute
	{16 + 6 * 12, 45, 16 + 8 * 12, 45}, // Second
};

/**
 * @brief 当前日历状态
 */
CalendarState calendarState = CalendarState_Normal;

/**
 * @brief 当前设置状态
 */
SettingState settingState = Year;

/**
 * @brief 设置时间结构体
 */
struct tm settingTime;

/**
 * @brief 旋钮顺时针旋转回调函数
 * @details 在设置模式下增加当前选中的时间项的值
 */
void onKnobForward(){
	char message[] = "Forward";
	HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
	if (calendarState == CalendarState_Setting){
		switch (settingState){
		case Year:
			settingTime.tm_year++;
			break;
		case Month:
			settingTime.tm_mon++;
			if (settingTime.tm_mon > 11){
				settingTime.tm_mon = 0;
			}
			break;
		case Day:
			settingTime.tm_mday++;
			if (settingTime.tm_mday > 31){
				settingTime.tm_mday = 1;
			}
			break;
		case Hour:
			settingTime.tm_hour++;
			if (settingTime.tm_hour > 23){
				settingTime.tm_hour = 0;
			}
			break;
		case Minute:
			settingTime.tm_min++;
			if (settingTime.tm_min > 59){
				settingTime.tm_min = 0;
			}
			break;
		case Second:
			settingTime.tm_sec++;
			if (settingTime.tm_sec > 59){
				settingTime.tm_sec = 0;
			}
			break;
		}
	}
}

/**
 * @brief 旋钮逆时针旋转回调函数
 * @details 在设置模式下减少当前选中的时间项的值
 */
void onKnobBackward(){
	char message[] = "Backward";
	HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
	if (calendarState == CalendarState_Setting){
		switch (settingState){
		case Year:
			settingTime.tm_year--;
			if (settingTime.tm_year < 70){
				settingTime.tm_year = 70;
			}
			break;
		case Month:
			settingTime.tm_mon--;
			if (settingTime.tm_mon < 0){
				settingTime.tm_mon = 11;
			}
			break;
		case Day:
			settingTime.tm_mday--;
			if (settingTime.tm_mday < 0){
				settingTime.tm_mday = 31;
			}
			break;
		case Hour:
			settingTime.tm_hour--;
			if (settingTime.tm_hour < 0){
				settingTime.tm_hour = 23;
			}
			break;
		case Minute:
			settingTime.tm_min--;
			if (settingTime.tm_min < 0){
				settingTime.tm_min = 59;
			}
			break;
		case Second:
			settingTime.tm_sec--;
			if (settingTime.tm_sec < 0){
				settingTime.tm_sec = 59;
			}
			break;
		}
	}
}

/**
 * @brief 旋钮按下回调函数
 * @details 在普通模式下进入设置模式，在设置模式下切换设置项或保存设置
 */
void onKnobPressed(){
	char message[] = "Pressed";
	HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
	if (calendarState == CalendarState_Normal){
		settingTime = *KK_RTC_GetTime();
		settingState = Year;
		calendarState = CalendarState_Setting;
	}else {
		if (settingState == Second){
			KK_RTC_SetTime(&settingTime);
			calendarState = CalendarState_Normal;
		} else{
			settingState++;
		}
	}
}

/**
 * @brief 显示时间函数
 * @param time 要显示的时间结构体指针
 * @details 在OLED上显示年月日、时分秒和星期几
 */
void showTime(struct tm* time){
	char str[30];
	sprintf(str, "%d-%02d-%02d", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday);
	OLED_PrintASCIIString(24, 0, str, &afont16x8, OLED_COLOR_NORMAL);

	sprintf(str, "%02d:%02d:%02d", time->tm_hour, time->tm_min, time->tm_sec);
	OLED_PrintASCIIString(16, 20, str, &afont24x12, OLED_COLOR_NORMAL);

	char* week = weeks[time->tm_wday];
	uint8_t x_week = (128 - (strlen(week) * 8)) / 2;
	OLED_PrintASCIIString(x_week, 48, week, &afont16x8, OLED_COLOR_NORMAL);
}

/**
 * @brief 显示光标函数
 * @details 在设置模式下显示闪烁的光标，指示当前正在设置的时间项
 */
void showCursor(){
	static uint32_t startTime = 0;
	uint32_t difftime = HAL_GetTick() - startTime;
	if (difftime > 2 * CURSOR_FLASH_INTERVAL){
		startTime = HAL_GetTick();
	}else if (difftime > CURSOR_FLASH_INTERVAL){
		CursorPosition position = cursorPositions[settingState];
		OLED_DrawLine(position.x1, position.y1, position.x2, position.y2, OLED_COLOR_NORMAL);
	}
}

/**
 * @brief 主任务初始化函数
 * @details 初始化OLED、RTC和旋钮，设置旋钮回调函数
 */
void MainTaskInit(){
	HAL_Delay(20);
	OLED_Init();
	KK_RTC_Init();
	Knob_Init();
	Knob_SetForwardCallback(onKnobForward);
	Knob_SetBackwardCallback(onKnobBackward);
	Knob_SetPressedCallback(onKnobPressed);
}

/**
 * @brief 主任务循环函数
 * @details 处理旋钮输入，更新显示内容
 */
void MainTask(){
	Knob_Loop();
	OLED_NewFrame();

	if (calendarState == CalendarState_Normal){
		struct tm* now = KK_RTC_GetTime();
		showTime(now);
	}else{
		showTime(&settingTime);
		showCursor();
	}

	OLED_ShowFrame();
}
