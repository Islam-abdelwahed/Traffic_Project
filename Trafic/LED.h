#ifndef LED_H_
#define LED_H_

#include "LED_CFG.h"
#include "DIO.h"
#include "STD.h"


void LED_Init(uint8 Color);
void LED_ON(uint8 Color);
void LED_OFF(uint8 Color);
void LED_Toggle(uint8 Color);


#endif /* LED_H_ */