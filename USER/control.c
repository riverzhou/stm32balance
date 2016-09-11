#include <stdio.h>
#include <stdlib.h>
#include "MPU6050.h"
#include "encoder.h"
#include "motor.h"
#include "battery.h"

#define BAL_ANGEL 	0
#define BAL_KP			300
#define BAL_KD			1

#define VEL_KP			80
#define VEL_KI			0.4

#define ENC_FILTE1 	0.9
#define ENC_FILTE2 	0.1

/**************************************************************************
函数功能：获取平衡角度和平衡角速度和转向角速度
入口参数：衡角度和平衡角速度和转向角速度
返回  值：无
**************************************************************************/
void Get_Angle(float* Bal_Angle, float* Bal_Gyro, float* Turn_Gyro)
{ 
	float Pitch, Gyro_Y, Gyro_Z;

	if(!Read_DMP(&Pitch, &Gyro_Y, &Gyro_Z)){
		*Bal_Angle = Pitch;
		*Bal_Gyro  = Gyro_Y;
		*Turn_Gyro = Gyro_Z;
	}
	else{
		*Bal_Angle = 0;
		*Bal_Gyro  = 0;
		*Turn_Gyro = 0;
	}
}

/**************************************************************************
函数功能：直立PD控制
入口参数：角度、角速度
返回  值：直立控制PWM
**************************************************************************/
int balance(float Angle, float Gyro)
{  
	return (Angle - BAL_ANGEL) * BAL_KP + Gyro * BAL_KD;
  //===求出平衡的角度中值 和机械相关
	//===计算平衡控制的电机PWM  PD控制
}

/**************************************************************************
函数功能：速度PI控制 修改前进后退速度，请修Target_Velocity，比如，改成60就比较慢了
入口参数：左轮编码器、右轮编码器
返回  值：速度控制PWM
**************************************************************************/
int velocity(int encoder_left,int encoder_right)
{  
	static float Encoder_Least=0.0f, Encoder=0.0f, Movement=0.0f, Encoder_Integral=0.0f;
	//=============速度PI控制器=======================//	
	Encoder_Least = (encoder_left + encoder_right) - 0;               		//===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
	Encoder *= ENC_FILTE1;		                                        		//===一阶低通滤波器       
	Encoder += Encoder_Least * ENC_FILTE2;	                          		//===一阶低通滤波器    
	Encoder_Integral += Encoder;                                      		//===积分出位移 积分时间：10ms
	Encoder_Integral = Encoder_Integral - Movement;                   		//===接收遥控器数据，控制前进后退
	if(Encoder_Integral>10000)  Encoder_Integral = 10000;            			//===积分限幅
	if(Encoder_Integral<-10000)	Encoder_Integral = -10000;            		//===积分限幅	
	return Encoder * VEL_KP + Encoder_Integral * VEL_KI;              		//===速度控制	
}

/**************************************************************************
函数功能：所有的控制代码都在这里面
         5ms定时中断由MPU6050的INT引脚触发
         严格保证采样和数据处理的时间同步				 
**************************************************************************/
int EXTI9_5_IRQHandler(void) 
{ 
	static int Flag_Target = 0;
	
	if(PBin(5)==0){
		EXTI->PR=1<<5;                                                      //清除LINE5上的中断标志位   
		
		float Bal_Angle=0.0f, Bal_Gyro=0.0f, Turn_Gyro=0.0f;								//平衡倾角 平衡陀螺仪 转向陀螺仪
		Get_Angle(&Bal_Angle, &Bal_Gyro, &Turn_Gyro);                       //===更新姿态	
		
		Flag_Target=!Flag_Target;
		if(Flag_Target==1)                                                  //5ms读取一次陀螺仪和加速度计的值，更高的采样频率可以改善卡尔曼滤波和互补滤波的效果
			return 0;	                                               
		
		int Voltage=0;
		Voltage=Get_battery_volt();
		
		int Balance_Pwm=0, Velocity_Pwm=0;
		int Motor1=0, Motor2=0;
		int Encoder_Left=0, Encoder_Right=0;             										//左右编码器的脉冲计数
		
		Encoder_Left  = -Read_Encoder(2);                                   //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
		Encoder_Right = Read_Encoder(4);                                    //===读取编码器的值
		Balance_Pwm   = balance(Bal_Angle, Bal_Gyro);                				//===平衡PID控制	
		Velocity_Pwm  = velocity(Encoder_Left, Encoder_Right);              //===速度环PID控制	 记住，速度反馈是正反馈，就是小车快的时候要慢下来就需要再跑快一点
		Motor1 = Balance_Pwm - Velocity_Pwm;                                //===计算左轮电机最终PWM
		Motor2 = Balance_Pwm - Velocity_Pwm;                                //===计算右轮电机最终PWM

		if(!Turn_Off(Bal_Angle,Voltage))                   					        //===检测倾角是否在许可范围内
			Set_Pwm(Motor1, Motor2);                                          //===赋值给PWM寄存器  

		//printf("Bal_Angle %f , Bal_Gyro %f , Motor1 %d , Motor2 %d \r\n", Bal_Angle, Bal_Gyro, Motor1, Motor2);
		
	}       	
	return 0;	  
} 

