/*
 * communication.c
 *
 *  Created on: Jun 11, 2023
 *      Author: xq123
 */
#include "main.h"
#include "communication.h"
#include "atk_ms901m.h"
#include "atk_ms901m_uart.h"
#include "motor_control.h"
#include "pid_control.h"
#include "stage.h"
#include "stdbool.h"

COM_CREATE(USART1)
COM_CREATE(USART3)
COM_CREATE(UART4)

uint16_t Communication_TX_Count=0;
uint16_t Communication_RX_Count=0;

atk_ms901m_attitude_data_t attitude_dat;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_uart4_rx;

void Communication_Unpack(UART_HandleTypeDef *huart)
{
    if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
    {
      __HAL_UART_CLEAR_IDLEFLAG(huart);
      HAL_UART_DMAStop(huart);
      if(huart->Instance == USART1)
      {
          USART1_RX_Length = BUFFER_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
          if(USART1_RX_Buffer[USART1_RX_Length-1]==USART1_RX_Length)
          {
        	  bias_error=(int8_t)USART1_RX_Buffer[1];
        	  if(USART1_RX_Buffer[3]==1)
        		  MV_stop_flag=true;
        	  else
        	  {
                  MV_stop_flag=false;
        	  }
        	  if(USART1_RX_Buffer[3]==2)
        		  MV_end_flag=true;
        	  else
        	  {
        	      MV_end_flag=false;
        	  }
          }
          HAL_UART_Receive_DMA(huart, USART1_RX_Buffer, BUFFER_LENGTH);
      }
      if(huart->Instance == USART3)
      {
          USART3_RX_Length = BUFFER_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
          if(USART3_RX_Buffer[USART3_RX_Length-1]==USART3_RX_Length)
          {
        	  bias_error=(int8_t)USART3_RX_Buffer[1];
        	  if(USART3_RX_Buffer[3]==1)
        		  MV_stop_flag=true;
        	  if(USART3_RX_Buffer[3]==2)
        		  MV_end_flag=true;
          }
          HAL_UART_Receive_DMA(huart, USART3_RX_Buffer, BUFFER_LENGTH);
      }
      if(huart->Instance == UART4)
      {
          UART4_RX_Length = BUFFER_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);

          atk_ms901m_uart_rx_fifo_write(UART4_RX_Buffer,UART4_RX_Length);

          atk_ms901m_get_attitude(&attitude_dat,0xff);

          Angle_gz=attitude_dat.yaw;
          if(Angle_gz<0)Angle_gz+=360;

          HAL_UART_Receive_DMA(huart, UART4_RX_Buffer, BUFFER_LENGTH);
      }
    }
}




