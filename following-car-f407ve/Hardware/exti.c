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

uint8_t USART1_RX_BUF[USART1_REC_LEN];//���ջ���,���USART_REC_LEN���ֽ�.
uint16_t USART1_RX_STA=0;//����״̬���//bit15��������ɱ�־��bit14~0�����յ�����Ч�ֽ���Ŀ
uint8_t USART1_NewData;//��ǰ�����жϽ��յ�1���ֽ����ݵĻ���
uint8_t obstacle_flag = 0;
int count=0;

#define OVERTAKE_CAR

void  HAL_UART_RxCpltCallback(UART_HandleTypeDef  *huart)//�����жϻص�����
{
	if(huart ==&huart1)
	{
    if((USART1_RX_STA&0x8000)==0)//����δ���
        {
            if(USART1_NewData==0x5A)//���յ���0x5A
            {
                 USART1_RX_STA|=0x8000;   //��������ˣ���USART2_RX_STA�е�bit15��15λ����1
            }
            else
            {
                   USART1_RX_BUF[USART1_RX_STA&0X7FFF]=USART1_NewData; /*���յ������ݷ������飬
                   ���簴�°���1��ǰ������
                   USART2_RX_BUF[0]=0xA5
                   USART2_RX_BUF[1]=0x01
                   USART2_RX_BUF[2]=0x01
                   ��Ȼ����ģ�鷢�͵����ݰ���4���ֽڵ��ǰ�β0x5A�����
    	           ��USART2_RX_BUF�У�����Ƭ�����յ���β��0x5Aʱ�ὫUSART2_RX_STA�����λ��1*/
                   USART1_RX_STA++;  //���ݳ��ȼ�����1
                   if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����
            }
        }
			HAL_UART_Receive_IT(&huart1,(uint8_t *)&USART1_NewData,1); //��Ϊÿִ����һ���жϻص������Ὣ�����жϹ��ܹرգ����������Ҫ�ٿ��������ж�  
   }
     
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	

	if(htim->Instance == TIM6)
	{	
//		  if(count>=0&&count<=500)
//		{
			//Turn.pGain=23;
			//Turn.dGain=30;
			//motor_pid_l.pGain=150;
			//motor_pid_l.iGain=14;
			//motor_pid_r.pGain=150;
			//motor_pid_r.iGain=14;
			Track(expect_speed);
//		}
//		count++;
//		if(count>501)
//		{
//			count=501;
//			Give_Motor_PWM(0,0);
//		}
	}
}
