#include "Atmega32_Registers.h"
#include "LCD.h"
#include "Buzzer.h"
#include "KEYPAD.h"
#include "LED.h"
#include "7_Segment.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Timer.h"
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>

extern uint8 Num_OVF;
extern uint8 Init_Val;

uint8 R_MIN,
R_SEC,
T_MIN,
T_SEC,
G_MIN,
G_SEC,
STATE=0;


void convert_to_time(char *ex,uint8 *MIN,uint8 *SEC){
	uint8 buffer[15];
	uint16 i,len, j, x;
	len = strlen(ex);
	j = 0;

	for (i = 0; i < len+1; i++) {

		if (ex[i] >= '0' && ex[i] <= '9') {
			buffer[j++] = ex[i];
		}
		else if (ex[i] == ':') {
			if (j > 0) {
				buffer[j] = '\0';
				x = atoi(buffer);
				*MIN=x;
				j = 0;
			}
			}else if (ex[i] == '\0') {
			if (j > 0) {
				buffer[j] = '\0';
				x = atoi(buffer);
				*SEC=x;
				j = 0;
			}
		}
	}
	
}


void get_time(uint8 *expression){
	_Bool x=true,y=false;
	uint8 index=0,i=0;
	while(x){
		uint8 t=keypadGetValue();
		if(t)
		{
			BUZZER_SHORT_BEEP();
			switch (t)
			{
				case 'c':
				index=0;
				LCD_CLR();
				break;

				case '=':
				if (expression[0] == '\0')
				break;
				
				x=false;
				break;
				case ':':
				if (y) break;
				
				y=true;
				i=0;
				expression[index] = t;
				LCD_WRITE_CHAR(t);
				index++;
				break;
				
				default:
				if (i==2)
				break;
				
				expression[index] = t;
				LCD_WRITE_CHAR(t);
				index++;
				i++;
				break;
			}
			expression[index] = '\0';
		}
	}
}

int main(void)
{
	uint8 expression[5],
	index=0;
	
	BUZZER_Init();
	KEYPAD_INIT();
	LCD_INIT();
	SET_BIT(DDRA,4);
	SET_BIT(DDRA,6);
	SET_BIT(DDRB,7);
	
	BUZZER_BEEP_BEEP();
	LOCATE_CURSOR(1,3);
	LCD_WRITE_STRING("Traffic light");
	_delay_ms(3000);
	LCD_CLR();
	
	
	LOCATE_CURSOR(0,3);
	LCD_WRITE_STRING("Enter Red Time");
	LOCATE_CURSOR(1,5);
	LCD_WRITE_STRING("e.g mm:ss");
	LOCATE_CURSOR(3,7);
	SET_BIT(PORTB,7);
	get_time(expression);
	convert_to_time(expression,&R_MIN,&R_SEC);
	CLR_BIT(PORTB,7);
	
	LCD_CLR();
	
	
	SET_BIT(PORTA,4);
	LOCATE_CURSOR(0,3);
	LCD_WRITE_STRING("Enter Green Time");
	LOCATE_CURSOR(1,5);
	LCD_WRITE_STRING("e.g mm:ss");
	LOCATE_CURSOR(3,7);
	get_time(expression);
	convert_to_time(expression,&G_MIN,&G_SEC);
	CLR_BIT(PORTA,4);
	
	LCD_CLR();
	
	Timer0_Init();
	Timer0_SetDelay(1000);
	Timer0_Start();
	_7segmentInit();
	T_MIN=R_MIN;
	T_SEC=R_SEC;
	SET_BIT(PORTB,7);
	while (1)
	{
		selectDigit(SEGMENT_COM4);
		setDigit(T_MIN/10);
		selectDigit(SEGMENT_COM3);
		setDigit(T_MIN%10);
		selectDigit(SEGMENT_COM2);
		setDigit(T_SEC/10);
		selectDigit(SEGMENT_COM1);
		setDigit(T_SEC%10);
	}
}

void Count_des()
{
	if(T_MIN==0 && T_SEC==0)
	{
		if(STATE==0){
			STATE=1;
			T_MIN=G_MIN;
			T_SEC=G_SEC;
			CLR_BIT(PORTB,7);
			SET_BIT(PORTA,4);
			}else if(STATE==1){
			STATE=0;
			T_MIN=R_MIN;
			T_SEC=R_SEC;
			CLR_BIT(PORTA,4);
			SET_BIT(PORTB,7);
		}
	}
	else if (T_SEC==0)
	{
		T_MIN--;
		T_SEC=60;
	}
	else if(T_MIN==0 && T_SEC<5){
		TOGGLE_BIT(PORTA,6);
		BUZZER_Toggle();
	}
	
}

ISR(TIMER0_OVF_vect)
{
	static uint16 count=0;
	
	count++;
	if (count==Num_OVF)
	{
		T_SEC--;
		Count_des();
		count=0;
		TCNT0=Init_Val;
	}
}

