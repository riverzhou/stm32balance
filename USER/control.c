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
�������ܣ���ȡƽ��ǶȺ�ƽ����ٶȺ�ת����ٶ�
��ڲ�������ǶȺ�ƽ����ٶȺ�ת����ٶ�
����  ֵ����
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
�������ܣ�ֱ��PD����
��ڲ������Ƕȡ����ٶ�
����  ֵ��ֱ������PWM
**************************************************************************/
int balance(float Angle, float Gyro)
{  
	return (Angle - BAL_ANGEL) * BAL_KP + Gyro * BAL_KD;
  //===���ƽ��ĽǶ���ֵ �ͻ�е���
	//===����ƽ����Ƶĵ��PWM  PD����
}

/**************************************************************************
�������ܣ��ٶ�PI���� �޸�ǰ�������ٶȣ�����Target_Velocity�����磬�ĳ�60�ͱȽ�����
��ڲ��������ֱ����������ֱ�����
����  ֵ���ٶȿ���PWM
**************************************************************************/
int velocity(int encoder_left,int encoder_right)
{  
	static float Encoder_Least=0.0f, Encoder=0.0f, Movement=0.0f, Encoder_Integral=0.0f;
	//=============�ٶ�PI������=======================//	
	Encoder_Least = (encoder_left + encoder_right) - 0;               		//===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
	Encoder *= ENC_FILTE1;		                                        		//===һ�׵�ͨ�˲���       
	Encoder += Encoder_Least * ENC_FILTE2;	                          		//===һ�׵�ͨ�˲���    
	Encoder_Integral += Encoder;                                      		//===���ֳ�λ�� ����ʱ�䣺10ms
	Encoder_Integral = Encoder_Integral - Movement;                   		//===����ң�������ݣ�����ǰ������
	if(Encoder_Integral>10000)  Encoder_Integral = 10000;            			//===�����޷�
	if(Encoder_Integral<-10000)	Encoder_Integral = -10000;            		//===�����޷�	
	return Encoder * VEL_KP + Encoder_Integral * VEL_KI;              		//===�ٶȿ���	
}

/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
         5ms��ʱ�ж���MPU6050��INT���Ŵ���
         �ϸ�֤���������ݴ����ʱ��ͬ��				 
**************************************************************************/
int EXTI9_5_IRQHandler(void) 
{ 
	static int Flag_Target = 0;
	
	if(PBin(5)==0){
		EXTI->PR=1<<5;                                                      //���LINE5�ϵ��жϱ�־λ   
		
		float Bal_Angle=0.0f, Bal_Gyro=0.0f, Turn_Gyro=0.0f;								//ƽ����� ƽ�������� ת��������
		Get_Angle(&Bal_Angle, &Bal_Gyro, &Turn_Gyro);                       //===������̬	
		
		Flag_Target=!Flag_Target;
		if(Flag_Target==1)                                                  //5ms��ȡһ�������Ǻͼ��ٶȼƵ�ֵ�����ߵĲ���Ƶ�ʿ��Ը��ƿ������˲��ͻ����˲���Ч��
			return 0;	                                               
		
		int Voltage=0;
		Voltage=Get_battery_volt();
		
		int Balance_Pwm=0, Velocity_Pwm=0;
		int Motor1=0, Motor2=0;
		int Encoder_Left=0, Encoder_Right=0;             										//���ұ��������������
		
		Encoder_Left  = -Read_Encoder(2);                                   //===��ȡ��������ֵ����Ϊ�����������ת��180�ȵģ����Զ�����һ��ȡ������֤�������һ��
		Encoder_Right = Read_Encoder(4);                                    //===��ȡ��������ֵ
		Balance_Pwm   = balance(Bal_Angle, Bal_Gyro);                				//===ƽ��PID����	
		Velocity_Pwm  = velocity(Encoder_Left, Encoder_Right);              //===�ٶȻ�PID����	 ��ס���ٶȷ�����������������С�����ʱ��Ҫ����������Ҫ���ܿ�һ��
		Motor1 = Balance_Pwm - Velocity_Pwm;                                //===�������ֵ������PWM
		Motor2 = Balance_Pwm - Velocity_Pwm;                                //===�������ֵ������PWM

		if(!Turn_Off(Bal_Angle,Voltage))                   					        //===�������Ƿ�����ɷ�Χ��
			Set_Pwm(Motor1, Motor2);                                          //===��ֵ��PWM�Ĵ���  

		//printf("Bal_Angle %f , Bal_Gyro %f , Motor1 %d , Motor2 %d \r\n", Bal_Angle, Bal_Gyro, Motor1, Motor2);
		
	}       	
	return 0;	  
} 

