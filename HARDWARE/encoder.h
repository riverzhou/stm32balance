#ifndef __ENCODER_H
#define __ENCODER_H
#include "stm32f10x.h"

void Encoder_Init(void);
int Read_Encoder(int TIMX);

#endif
