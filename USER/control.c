#include <stdio.h>
#include "MPU6050.h"

//int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数
//int Moto1,Moto2;                            //电机PWM变量 应是Motor的 向Moto致敬	

float Angle_Balance,Gyro_Balance;           //平衡倾角 平衡陀螺仪 转向陀螺仪
u8 Flag_Target;

/**************************************************************************
函数功能：获取角度 三种算法经过我们的调校，都非常理想 
入口参数：获取角度的算法 1：DMP  2：卡尔曼 3：互补滤波
返回  值：无
**************************************************************************/
void Get_Angle(void)
{ 
	Read_DMP();                      //===读取加速度、角速度、倾角
	Angle_Balance = Pitch;             //===更新平衡倾角
	Gyro_Balance  = gyro[1];            //===更新平衡角速度
}

/**************************************************************************
函数功能：所有的控制代码都在这里面
         5ms定时中断由MPU6050的INT引脚触发
         严格保证采样和数据处理的时间同步				 
**************************************************************************/
int EXTI9_5_IRQHandler(void) 
{    
	if(PBin(5)==0){
		EXTI->PR=1<<5;                                                        //清除LINE5上的中断标志位   
		Flag_Target=!Flag_Target;
		if(Flag_Target==1){                                                   //5ms读取一次陀螺仪和加速度计的值，更高的采样频率可以改善卡尔曼滤波和互补滤波的效果
			Get_Angle();                                                        //===更新姿态	
			return 0;	                                               
		}
#if 0																																		//10ms控制一次，为了保证M法测速的时间基准，首先读取编码器数据
		Encoder_Left=-Read_Encoder(2);                                      //===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
		Encoder_Right=Read_Encoder(4);                                      //===读取编码器的值
	  Get_Angle();                                                        //===更新姿态	                                                            //===扫描按键状态 单击双击可以改变小车运行状态
 		Balance_Pwm =balance(Angle_Balance,Gyro_Balance);                   //===平衡PID控制	
		Velocity_Pwm=velocity(Encoder_Left,Encoder_Right);                  //===速度环PID控制	 记住，速度反馈是正反馈，就是小车快的时候要慢下来就需要再跑快一点
 	  Moto1=Balance_Pwm-Velocity_Pwm;                                     //===计算左轮电机最终PWM
 	  Moto2=Balance_Pwm-Velocity_Pwm;                                     //===计算右轮电机最终PWM
   	Limit_Pwm();                                                        //===PWM限幅
    if(Turn_Off(Angle_Balance)==0)                                      //===如果不存在异常
 		Set_Pwm(Moto1,Moto2);                                               //===赋值给PWM寄存器  
#endif
		Get_Angle();
		printf("Angle_Balance %f , Gyro_Balance %f \r\n", Angle_Balance, Gyro_Balance);
	}       	
	return 0;	  
} 

