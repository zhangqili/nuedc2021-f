/*
 * exti.h
 *
 *  Created on: Aug 17, 2022
 *      Author: HP
 */

#ifndef EXTI_EXTI_H_
#define EXTI_EXTI_H_
#include "main.h"
#include "stm32f4xx_hal.h" //HAL���ļ�����
extern UART_HandleTypeDef huart1;//����USART2��HAL��ṹ��
extern int count;
#define USART1_REC_LEN  200//����USART2�������ֽ���

extern uint8_t  USART1_RX_BUF[USART1_REC_LEN];//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�ΪУ���
extern uint16_t USART1_RX_STA;//����״̬���
extern uint8_t USART1_NewData;//��ǰ�����жϽ��յ�1���ֽ����ݵĻ���
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef  *huart);//�����жϻص���������

#endif /* EXTI_EXTI_H_ */
