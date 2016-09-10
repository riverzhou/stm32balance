/*----------------------------------------------------------------------------
 * Name:    Serial.h
 * Purpose: Low level serial definitions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef SERIAL_H
#define SERIAL_H
#include "stm32f10x.h"

extern void SER_Init(u32 pclk2, u32 bound);
extern int  SER_GetChar(void);
extern int  SER_PutChar(int c);
extern int 	SER_Get(unsigned char *data);
extern void SER_Put(unsigned char c);
#endif
