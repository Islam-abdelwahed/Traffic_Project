#include "Timer.h"

 uint8 Num_OVF=0;

 uint8 Init_Val=0;

void Timer0_Init(void){
	
	#if TIMER0_MODE == TIMER0_NORMAL_MODE
	SET_BIT(TCCR0,6);
	SET_BIT(TCCR0,3);
	
	#elif TIMER0_MODE == TIMER0_PWM_MODE
	SET_BIT(TCCR0,6);
	SET_BIT(TCCR0,3);
	 
	#elif TIMER0_MODE == TIMER0_CTC_MODE
	SET_BIT(TCCR0,6);
	SET_BIT(TCCR0,3);
	
	#elif TIMER0_MODE == TIMER0_FAST_PWM_MODE
	SET_BIT(TCCR0,6);
	SET_BIT(TCCR0,3);
	
	#endif
	
	TIMSK |= 0x01;

	SREG  |= 0x80;

	};

void Timer0_Start(void){
	TCCR0 =0x05;
	
	};
	

void Timer0_Stop(void){
	CLR_BIT(TCCR0,0);
	CLR_BIT(TCCR0,1);
	CLR_BIT(TCCR0,2);
	};

void Timer0_SetDelay(uint32 Delay_Ms){
	
	uint8 Tick_Time =(1024/16);
	
	uint32 Total_Ticks= (Delay_Ms*1000) /Tick_Time;
	
	Init_Val= 256 - (Total_Ticks%256);
	
	Num_OVF =Total_Ticks/256 ;
	
	TCNT0=Init_Val;
	Num_OVF++;
	};
