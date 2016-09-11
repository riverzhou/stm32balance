#ifndef _BATTERY_H_
#define _BATTERY_H_
#include "stm32f10x.h"

#define Battery_Ch 4
void Battery_Init(void);
int Get_battery_volt(void);   

#endif 


