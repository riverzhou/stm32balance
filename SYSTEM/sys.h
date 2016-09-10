#ifndef __SYS_H
#define __SYS_H	 
#include "stm32f10x.h"

//Ex_NVIC_Configר�ö���
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6 

#define FTIR   1  //�½��ش���
#define RTIR   2  //�����ش���

void MY_NVIC_Init(u32 NVIC_PreemptionPriority,u32 NVIC_SubPriority,u32 NVIC_Channel,u32 NVIC_Group);//�����ж�
void Ex_NVIC_Config(u32 GPIOx,u32 BITx,u32 TRIM);//�ⲿ�ж����ú���(ֻ��GPIOA~G)

#endif
