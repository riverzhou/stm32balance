#include<stdio.h>
#include "Serial.h"
#include "delay.h"
#include "ioi2c.h"
#include "exti.h"
#include "MPU6050.h"
#include "motor.h"
#include "encoder.h"

int main(void)
{
	delay_init(72);                 //=====��ʱ��ʼ��
	SER_Init(72, 115200);						//=====��ʼ������1
  Motor_Init(7199,0);   					//=====��ʼ��PWM 10KHZ������������� �����ʼ������ӿ� ��ΪMotor_Init(9999,35) 200HZ
	Encoder_Init();            			//=====�������ӿ�
	IIC_Init();                     //=====ģ��IIC��ʼ��
  DMP_Init();                     //=====��ʼ��DMP     
	EXTI_Init();                    //=====MPU6050 5ms��ʱ�жϳ�ʼ��
	
	printf("hello world\n");
	
	while(1){
		__NOP();
	}
}
