#include "task_main.h"

#define StateLineFlashInterval 500
#define _GetSysTick HAL_GetTick

typedef enum {
  CLOCK_STATE_NORMAL,  // 普通模式
  CLOCK_STATE_SETTING, // 设置模式
} ClockState;

typedef struct {
  uint8_t x1;
  uint8_t y1;
  uint8_t x2;
  uint8_t y2;
} SettingLine;

ClockState clockState = CLOCK_STATE_NORMAL;

typedef enum { Year = 0, Month, Day, Hour, Minute, Second } SettingState;

SettingState settingState = Year;
SettingLine settingLines[6] = {
    {24 + 0 * 8, 17, 24 + 4 * 8, 17},   // Year
    {24 + 5 * 8, 17, 24 + 7 * 8, 17},   // Month
    {24 + 8 * 8, 17, 24 + 10 * 8, 17},  // Day
    {16 + 0 * 12, 45, 16 + 2 * 12, 45}, // Hour
    {16 + 3 * 12, 45, 16 + 5 * 12, 45}, // Minute
    {16 + 6 * 12, 45, 16 + 8 * 12, 45}, // Second
};
struct tm settingTime;

char week[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

char str[100];

void onKnobForward() {
  if (clockState == CLOCK_STATE_SETTING) {
    switch (settingState) {
    case Year:
      settingTime.tm_year++;
      break;
    case Month:
      settingTime.tm_mon++;
      if (settingTime.tm_mon > 11) {
        settingTime.tm_mon = 0;
      }
      break;
    case Day:
      settingTime.tm_mday++;
      if (settingTime.tm_mday > 31) {
        settingTime.tm_mday = 1;
      }
      break;
    case Hour:
      settingTime.tm_hour++;
      if (settingTime.tm_hour > 23) {
        settingTime.tm_hour = 0;
      }

      break;
    case Minute:
      settingTime.tm_min++;
      if (settingTime.tm_min > 59) {
        settingTime.tm_min = 0;
      }
      break;
    case Second:
      settingTime.tm_sec++;
      if (settingTime.tm_sec > 59) {
        settingTime.tm_sec = 0;
      }
      break;
    }
  }
}

void onKnobBackward() {
  if (clockState == CLOCK_STATE_SETTING) {
    switch (settingState) {
    case Year:
      settingTime.tm_year--;
      if (settingTime.tm_year < 70) {
        settingTime.tm_year = 70;
      }
      break;
    case Month:
      settingTime.tm_mon--;
      if (settingTime.tm_mon < 0) {
        settingTime.tm_mon = 11;
      }
      break;
    case Day:
      settingTime.tm_mday--;
      if (settingTime.tm_mday < 1) {
        settingTime.tm_mday = 31;
      }
      break;
    case Hour:
      settingTime.tm_hour--;
      if (settingTime.tm_hour < 0) {
        settingTime.tm_hour = 23;
      }
      break;
    case Minute:
      settingTime.tm_min--;
      if (settingTime.tm_min < 0) {
        settingTime.tm_min = 59;
      }
      break;
    case Second:
      settingTime.tm_sec--;
      if (settingTime.tm_sec < 0) {
        settingTime.tm_sec = 59;
      }
      break;
    }
  }
}

void onKnobBtnPress() {
  if (clockState == CLOCK_STATE_NORMAL) {
    clockState = CLOCK_STATE_SETTING;
    settingState = Year;
    settingTime = *KK_RTC_GetTime();
  } else {
    if (settingState == Second) {
      clockState = CLOCK_STATE_NORMAL;
      KK_RTC_SetTime(&settingTime);
    } else {
      settingState++;
    }
  }
}

void TaskMain_Init() {
  HAL_Delay(20);
  OLED_Init();
  Knob_Init();
  Knob_SetOnForwardCallback(onKnobForward);
  Knob_SetOnBackwardCallback(onKnobBackward);
  Knob_SetOnBtnPressCallback(onKnobBtnPress);
}

void _ShowTime(struct tm *time) {
  // 日期
  sprintf(str, "%d-%02d-%02d", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday);
  OLED_PrintASCIIString(24, 0, str, &afont16x8, OLED_COLOR_NORMAL);
  // 时间
  sprintf(str, "%02d:%02d:%02d", time->tm_hour, time->tm_min, time->tm_sec);
  OLED_PrintASCIIString(16, 20, str, &afont24x12, OLED_COLOR_NORMAL);
  // 星期
  uint8_t x_week = (128 - (strlen(week[time->tm_wday]) * 8)) / 2;
  OLED_PrintASCIIString(x_week, 48, week[time->tm_wday], &afont16x8, OLED_COLOR_NORMAL);
}

void _ShowStateLine() {
  static uint32_t lastFlashTime = 0;
  uint32_t difftime = _GetSysTick() - lastFlashTime;
  if (difftime > 2 * StateLineFlashInterval) {
    lastFlashTime = _GetSysTick();
  } else if (difftime > StateLineFlashInterval) {
    SettingLine *line = &settingLines[settingState];
    OLED_DrawLine(line->x1, line->y1, line->x2, line->y2, OLED_COLOR_NORMAL);
  }
}

void TaskMain_Loop() {
  Knob_TaskLoop();
  OLED_NewFrame();
  if (clockState == CLOCK_STATE_NORMAL) {
    // 普通模式
    struct tm *time = KK_RTC_GetTime();
    _ShowTime(time);
  } else {
    // 设置模式
    _ShowTime(&settingTime);
    // 画设置状态
    _ShowStateLine();
  }
  OLED_ShowFrame();
}
