#ifndef TIMER_H_
#define TIMER_H_
#include "Timer_CFG.h"
#include "Atmega32_Registers.h"
#include "Bit_Math.h"

#define TIMER0_NORMAL_MODE 0

#define TIMER0_PWM_MODE 1

#define TIMER0_CTC_MODE 2

#define TIMER0_FAST_PWM_MODE 3



void Timer0_Init(void);

void Timer0_Start(void);

void Timer0_Stop(void);

void Timer0_SetDelay(uint32 Delay_Ms);




#endif /* TIMER_H_ */