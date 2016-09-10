#include<stdio.h>
#include "Serial.h"
#include "delay.h"
#include "ioi2c.h"
#include "exti.h"
#include "MPU6050.h"

int main(void)
{
	delay_init(72);                 //=====延时初始化
	SER_Init(72, 115200);						//=====初始化串口1
	IIC_Init();                     //=====模拟IIC初始化
  DMP_Init();                     //=====初始化DMP     
	EXTI_Init();                    //=====MPU6050 5ms定时中断初始化
	
	printf("hello world\n");
	
	while(1){
		__NOP();
	}
}
