#include <stdlib.h>
#include "motor.h"
#include "ioi2c.h"

#define PWMA   TIM1->CCR1  //PA8
#define PWMB   TIM1->CCR4  //PA11

#define AIN2   PBout(15)
#define AIN1   PBout(14)
#define BIN1   PBout(13)
#define BIN2   PBout(12)

void Motor_Init(u16 arr,u16 psc)
{		 					 
	RCC->APB2ENR|=1<<3;        //PORTB时钟使能   
	GPIOB->CRH&=0X0000FFFF;    //PORTB12 13 14 15推挽输出
	GPIOB->CRH|=0X22220000;    //PORTB12 13 14 15推挽输出
	
	RCC->APB2ENR|=1<<11;       //使能TIM1时钟    
	RCC->APB2ENR|=1<<2;        //PORTA时钟使能     
	GPIOA->CRH&=0XFFFF0FF0;    //PORTA8 11复用输出
	GPIOA->CRH|=0X0000B00B;    //PORTA8 11复用输出
	TIM1->ARR=arr;             //设定计数器自动重装值 
	TIM1->PSC=psc;             //预分频器不分频
	TIM1->CCMR2|=6<<12;        //CH4 PWM1模式	
	TIM1->CCMR1|=6<<4;         //CH1 PWM1模式	
	TIM1->CCMR2|=1<<11;        //CH4预装载使能	 
	TIM1->CCMR1|=1<<3;         //CH1预装载使能	  
	TIM1->CCER|=1<<12;         //CH4输出使能	   
	TIM1->CCER|=1<<0;          //CH1输出使能	
	TIM1->BDTR |= 1<<15;       //TIM1必须要这句话才能输出PWM
	TIM1->CR1=0x8000;          //ARPE使能 
	TIM1->CR1|=0x01;           //使能定时器1 			
} 

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int moto1,int moto2)
{
	int Amplitude=7000;    //===PWM满幅是7200 限制在7000
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
函数功能：异常关闭电机
入口参数：倾角和电压
返回  值：1：异常  0：正常
**************************************************************************/
int Turn_Off(float angle)
{
	if(angle<-40||angle>40)
	{	                                                 //===倾角大于40度关闭电机
		AIN1=0;                                            //===可自行增加主板温度过高时关闭电机
		AIN2=0;
		BIN1=0;
		BIN2=0;
		return 1;
	}
  return 0;			
}

