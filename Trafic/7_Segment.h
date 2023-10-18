
#ifndef _SEGMENT_H_
#define _SEGMENT_H_
#include "7_Segment_Cfg.h"
#include "STD.h"
#include "BIT_MATH.h"


void _7segmentInit();
void setDigit(uint8 num);
void selectDigit(uint8 digit);

#endif /* 7_SEGMENT_H_ */