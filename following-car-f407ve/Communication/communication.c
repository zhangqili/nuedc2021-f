/*
 * communication.c
 *
 *  Created on: Jun 11, 2023
 *      Author: xq123
 */
#include "main.h"
#include "communication.h"
#include "atk_ms901m_uart.h"

uint8_t UART4_RX_Buffer[BUFFER_LENGTH];
uint8_t UART4_RX_Length=0;
uint8_t UART4_TX_Buffer[BUFFER_LENGTH];
uint8_t UART4_TX_Length=0;
uint16_t Communication_TX_Count=0;
uint16_t Communication_RX_Count=0;

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

          HAL_UART_Receive_DMA(huart, UART4_RX_Buffer, BUFFER_LENGTH);
      }
    }
}




