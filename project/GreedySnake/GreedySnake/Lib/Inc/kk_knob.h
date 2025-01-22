#ifndef INC_KK_KNOB_H_
#define INC_KK_KNOB_H_

#include "tim.h"

typedef void (*KnobCallback)(void);

void Knob_Init();
void Knob_Loop();
void Knob_SetForwardCallback(KnobCallback callback);
void Knob_SetBackwardCallback(KnobCallback callback);
void Knob_SetPressedCallback(KnobCallback callback);

#endif /* INC_KK_KNOB_H_ */
