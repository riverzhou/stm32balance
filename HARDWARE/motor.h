#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x.h"

void Motor_Init(u16 arr,u16 psc);
void Set_Pwm(int moto1,int moto2);
int  Turn_Off(float angle, int voltage);
	
#endif
