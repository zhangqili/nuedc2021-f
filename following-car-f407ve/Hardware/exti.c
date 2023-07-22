/*
 * exti.c
 *
 *  Created on: Aug 17, 2022
 *      Author: HP
 */

#include "exti.h"
#include "motor_control.h"
#include "mpu6050.h"
#include "pid_control.h"
#include "usart.h"
#include "main.h"
#include "stage.h"

uint8_t USART1_RX_BUF[USART1_REC_LEN];//���ջ���,���USART_REC_LEN���ֽ�.
uint16_t USART1_RX_STA=0;//����״̬���//bit15��������ɱ�־��bit14~0�����յ�����Ч�ֽ���Ŀ
uint8_t USART1_NewData;//��ǰ�����жϽ��յ�1���ֽ����ݵĻ���
uint8_t obstacle_flag = 0;
int count=0;

#define OVERTAKE_CAR

void  HAL_UART_RxCpltCallback(UART_HandleTypeDef  *huart)//�����жϻص�����
{
     
}


//************************************************//

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//��inv_mpu.h���õ�mpu6050������Ϊ100Hz,��10ms����һ���ⲿ�ж�
//{
////	switch(GPIO_Pin)
////	{
////		case INT_Pin :

////		   MPU_DMP_Get_Data(&pitch,&roll,&yaw);//�õ�������pitch
////		   MPU_Get_Gyroscope(&gx,&gy,&gz);    //�õ�y����ٶ�ֵ
////		   Get_Encoder(); //�õ����ұ���������ֵ�����ҵ��ת��
////		   balance_up=Up_balance(pitch,gy,Mechanical_Angle); //ֱ����
////		   velocity=Velocity( Encoder_Left,Encoder_Right,Mechanical_velocity); //�ٶȻ�
////		   turn_out=Turn_out(yaw,turn_speed);
////		   PWMA= balance_up+velocity-turn_out; //����ֱ�������ٶȻ�
////		   PWMB= balance_up+velocity+turn_out; 
////		   Limit(&PWMA,&PWMB);        //PWM�޷�
////		   if(pitch<-30||pitch>30)    //��鸩���ǵĽǶ�ֵ��������������жϵ�Ŀ���ǣ���С��ʧȥƽ��������رյ��
////		   {
////			   PWMA=0;
////			   PWMB=0;
////		   }
////		   Give_Motor_PWM(PWMA,PWMB); //��ֵ��PWM�Ĵ����Լ����Ƶ������ת

//////		 printf("pitch:%f\t,roll:%f\t,yaw:%f\r\n",pitch,roll,yaw);�����õ�

////			break;
////		default:
////			break;
////  }
//}
//int a=0 ,b=0;
//uint16_t pathlength=0;
uint32_t count1 = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	

	if(htim->Instance == TIM6)
	{	
//		  if(count>=0&&count<=500)
//		{
			//Turn.pGain=23;
			//Turn.dGain=30;
//			local.pGain=1;
	        Turn.pGain=20;
			motor_pid_l.pGain=100;
			motor_pid_l.iGain=20;
			motor_pid_r.pGain=100;
			motor_pid_r.iGain=20;
			//expectlength=600;
			//turn_left_speed();
			model_select();
//			track_flag=1;
//			turn_flag=0;
//			if((Angle_gz>90)&&(Angle_gz<100))
//			{
//			    track_flag=0;
//			    turn_flag=0;
//			}
            //Get_Speed();
            //Track(20);
			Get_Speed();
            follow_speed_adjust();
            //local_speed_speed();
            /*
#define INTERVAL_1 200
			count1++;
			if(count1<INTERVAL_1*1)
			{
	              Give_Motor_PWM(4000, 0);
			}
			else if(count1<INTERVAL_1*2)
			{
	              Give_Motor_PWM(-4000, 0);
			}
            else if(count1<INTERVAL_1*3)
            {
                  Give_Motor_PWM(0, 4000);
            }
            else if(count1<INTERVAL_1*4)
            {
                  Give_Motor_PWM(0, -4000);
            }
            else if(count1<INTERVAL_1*5)
            {
                  Give_Motor_PWM(4000, 4000);
            }
            else if(count1<INTERVAL_1*6)
            {
                  Give_Motor_PWM(-4000, 4000);
            }
            else if(count1<INTERVAL_1*7)
            {
                  Give_Motor_PWM(4000, -4000);
            }
            else if(count1<INTERVAL_1*8)
            {
                  Give_Motor_PWM(-4000, -4000);
            }
            */
//		}
//		count++;
//		if(count>501)
//		{
//			count=501;
//			Give_Motor_PWM(0,0);
//		}
	}
}
