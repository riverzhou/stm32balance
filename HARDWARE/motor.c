#include <stdlib.h>
#include "motor.h"
#include "ioi2c.h"

#define MAX_ANGEL			40				// 40��
#define MIN_VOLTAGE		1110			// 11.1V

#define PWMA   TIM1->CCR1  //PA8
#define PWMB   TIM1->CCR4  //PA11

#define AIN2   PBout(15)
#define AIN1   PBout(14)
#define BIN1   PBout(13)
#define BIN2   PBout(12)

void Motor_Init(u16 arr,u16 psc)
{		 					 
	RCC->APB2ENR|=1<<3;        //PORTBʱ��ʹ��   
	GPIOB->CRH&=0X0000FFFF;    //PORTB12 13 14 15�������
	GPIOB->CRH|=0X22220000;    //PORTB12 13 14 15�������
	
	RCC->APB2ENR|=1<<11;       //ʹ��TIM1ʱ��    
	RCC->APB2ENR|=1<<2;        //PORTAʱ��ʹ��     
	GPIOA->CRH&=0XFFFF0FF0;    //PORTA8 11�������
	GPIOA->CRH|=0X0000B00B;    //PORTA8 11�������
	TIM1->ARR=arr;             //�趨�������Զ���װֵ 
	TIM1->PSC=psc;             //Ԥ��Ƶ������Ƶ
	TIM1->CCMR2|=6<<12;        //CH4 PWM1ģʽ	
	TIM1->CCMR1|=6<<4;         //CH1 PWM1ģʽ	
	TIM1->CCMR2|=1<<11;        //CH4Ԥװ��ʹ��	 
	TIM1->CCMR1|=1<<3;         //CH1Ԥװ��ʹ��	  
	TIM1->CCER|=1<<12;         //CH4���ʹ��	   
	TIM1->CCER|=1<<0;          //CH1���ʹ��	
	TIM1->BDTR |= 1<<15;       //TIM1����Ҫ��仰�������PWM
	TIM1->CR1=0x8000;          //ARPEʹ�� 
	TIM1->CR1|=0x01;           //ʹ�ܶ�ʱ��1 			
} 

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int moto1,int moto2)
{
	int Amplitude=7000;    //===PWM������7200 ������7000
	int Moto1=moto1, Moto2=Moto2;
	
  if(Moto1<-Amplitude) Moto1=-Amplitude;	
	if(Moto1>Amplitude)  Moto1=Amplitude;	
	if(Moto2<-Amplitude) Moto2=-Amplitude;	
	if(Moto2>Amplitude)  Moto2=Amplitude;		
	
	if(Moto1 < 0)			
		AIN2=1, AIN1=0;
	else 	          
		AIN2=0, AIN1=1;
	PWMA = abs(Moto1);
	if(Moto2 < 0)	
		BIN1=0, BIN2=1;
	else        
		BIN1=1, BIN2=0;
	PWMB = abs(Moto2);	
}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������Ǻ͵�ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
int Turn_Off(float angle, int voltage)
{
	if(angle < -MAX_ANGEL || angle > MAX_ANGEL || voltage < MIN_VOLTAGE)
	{	                                                 //===��Ǵ���40�ȹرյ�������ߵ�ѹ����11.1V
		AIN1=0;                                          //===���������������¶ȹ���ʱ�رյ��
		AIN2=0;
		BIN1=0;
		BIN2=0;
		return 1;
	}
  return 0;			
}

