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
COM_CREATE(UART5)

uint16_t Communication_TX_Count=0;
uint16_t Communication_RX_Count=0;

atk_ms901m_attitude_data_t attitude_dat;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart5_rx;

void Communication_Unpack(UART_HandleTypeDef *huart)
{
    if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
    {
      __HAL_UART_CLEAR_IDLEFLAG(huart);
      HAL_UART_DMAStop(huart);
      if(huart->Instance == USART1)
      {
          Communication_RX_Count++;
          USART1_RX_Length = BUFFER_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
          printf("%s\n",USART1_RX_Buffer);
          switch (USART1_RX_Buffer[0])
          {
              case 'P':
                  K210_stop_flag = true;
                  break;
              case 'S':
                  num_flag=true;
                  turn_dir = 0;
                  break;
              case 'L':
                  num_flag=true;
                  turn_dir = 1;
                  break;
              case 'R':
                  num_flag=true;
                  turn_dir = 2;
                  break;
              default:
                  number = USART1_RX_Buffer[0]-'0';
                  break;
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
                  {
                      LOG_S(MV,true);
                      MV_stop_flag=true;
                  }
                  if(USART3_RX_Buffer[3]==2)
                  {
                      //LOG_S(end,true);
                      MV_end_flag=true;
                  }
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
      if(huart->Instance == UART5)
      {
          UART5_RX_Length = BUFFER_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_uart5_rx);

          switch(UART5_RX_Buffer[0])
          {
              case 0:
                  deliver_return_flag = true;
                  break;
              default:
                  break;
          }

          HAL_UART_Receive_DMA(huart, UART5_RX_Buffer, BUFFER_LENGTH);
      }
    }
}




