#ifndef INC_KK_KNOB_H_
#define INC_KK_KNOB_H_

#include "tim.h"

typedef void (*KnobCallback)(void);

void Knob_Init();
void Knob_TaskLoop();

void Knob_SetOnForwardCallback(KnobCallback callback);
void Knob_SetOnBackwardCallback(KnobCallback callback);
void Knob_SetOnBtnPressCallback(KnobCallback callback);

#endif /* INC_KK_KNOB_H_ */
