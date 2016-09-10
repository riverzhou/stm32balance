#include <stdio.h>
#include "MPU6050.h"

//int Encoder_Left,Encoder_Right;             //���ұ��������������
//int Moto1,Moto2;                            //���PWM���� Ӧ��Motor�� ��Moto�¾�	

float Angle_Balance,Gyro_Balance;           //ƽ����� ƽ�������� ת��������
u8 Flag_Target;

/**************************************************************************
�������ܣ���ȡ�Ƕ� �����㷨�������ǵĵ�У�����ǳ����� 
��ڲ�������ȡ�Ƕȵ��㷨 1��DMP  2�������� 3�������˲�
����  ֵ����
**************************************************************************/
void Get_Angle(void)
{ 
	Read_DMP();                      //===��ȡ���ٶȡ����ٶȡ����
	Angle_Balance = Pitch;             //===����ƽ�����
	Gyro_Balance  = gyro[1];            //===����ƽ����ٶ�
}

/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
         5ms��ʱ�ж���MPU6050��INT���Ŵ���
         �ϸ�֤���������ݴ����ʱ��ͬ��				 
**************************************************************************/
int EXTI9_5_IRQHandler(void) 
{    
	if(PBin(5)==0){
		EXTI->PR=1<<5;                                                        //���LINE5�ϵ��жϱ�־λ   
		Flag_Target=!Flag_Target;
		if(Flag_Target==1){                                                   //5ms��ȡһ�������Ǻͼ��ٶȼƵ�ֵ�����ߵĲ���Ƶ�ʿ��Ը��ƿ������˲��ͻ����˲���Ч��
			Get_Angle();                                                        //===������̬	
			return 0;	                                               
		}
#if 0																																		//10ms����һ�Σ�Ϊ�˱�֤M�����ٵ�ʱ���׼�����ȶ�ȡ����������
		Encoder_Left=-Read_Encoder(2);                                      //===��ȡ��������ֵ����Ϊ�����������ת��180�ȵģ����Զ�����һ��ȡ������֤�������һ��
		Encoder_Right=Read_Encoder(4);                                      //===��ȡ��������ֵ
	  Get_Angle();                                                        //===������̬	                                                            //===ɨ�谴��״̬ ����˫�����Ըı�С������״̬
 		Balance_Pwm =balance(Angle_Balance,Gyro_Balance);                   //===ƽ��PID����	
		Velocity_Pwm=velocity(Encoder_Left,Encoder_Right);                  //===�ٶȻ�PID����	 ��ס���ٶȷ�����������������С�����ʱ��Ҫ����������Ҫ���ܿ�һ��
 	  Moto1=Balance_Pwm-Velocity_Pwm;                                     //===�������ֵ������PWM
 	  Moto2=Balance_Pwm-Velocity_Pwm;                                     //===�������ֵ������PWM
   	Limit_Pwm();                                                        //===PWM�޷�
    if(Turn_Off(Angle_Balance)==0)                                      //===����������쳣
 		Set_Pwm(Moto1,Moto2);                                               //===��ֵ��PWM�Ĵ���  
#endif
		Get_Angle();
		printf("Angle_Balance %f , Gyro_Balance %f \r\n", Angle_Balance, Gyro_Balance);
	}       	
	return 0;	  
} 

