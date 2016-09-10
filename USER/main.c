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
	delay_init(72);                 //=====延时初始化
	SER_Init(72, 115200);						//=====初始化串口1
  Motor_Init(7199,0);   					//=====初始化PWM 10KHZ，用于驱动电机 如需初始化电调接口 改为Motor_Init(9999,35) 200HZ
	Encoder_Init();            			//=====编码器接口
	IIC_Init();                     //=====模拟IIC初始化
  DMP_Init();                     //=====初始化DMP     
	EXTI_Init();                    //=====MPU6050 5ms定时中断初始化
	
	printf("hello world\n");
	
	while(1){
		__NOP();
	}
}
