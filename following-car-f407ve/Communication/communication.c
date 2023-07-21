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

COM_CREATE(UART4)

uint16_t Communication_TX_Count=0;
uint16_t Communication_RX_Count=0;

atk_ms901m_attitude_data_t attitude_dat;

extern DMA_HandleTypeDef hdma_uart4_rx;

void Communication_Unpack(UART_HandleTypeDef *huart)
{
    if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
    {
      __HAL_UART_CLEAR_IDLEFLAG(huart);
      HAL_UART_DMAStop(huart);
      if(huart->Instance == UART4)
      {
          UART4_RX_Length = BUFFER_LENGTH - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);

          atk_ms901m_uart_rx_fifo_write(UART4_RX_Buffer,UART4_RX_Length);

          atk_ms901m_get_attitude(&attitude_dat,0xff);

          Angle_gz=attitude_dat.yaw;

          HAL_UART_Receive_DMA(huart, UART4_RX_Buffer, BUFFER_LENGTH);

      }
    }
}




