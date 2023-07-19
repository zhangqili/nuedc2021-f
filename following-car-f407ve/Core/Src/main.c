/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "exti.h"
#include "string.h"
#include "display.h"
#include "string.h"
#include "display.h"
#include "stdlib.h"
#include "stdio.h"
#include "pid_control.h"
#include "motor_control.h"
#include "flash.h"
#include "fezui.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
float Up_balance_KP = 200;
float Up_balance_KD = 1.1;
float Velocity_KP = -53;
float Velocity_KI = -0.265;
float Turn_KP = -20;
float Turn_KD = -0.6;
float Mechanical_Angle = -5.5;
int Mechanical_velocity = 0;
int turn_speed = 0;
int Encoder_Left=0;
int Encoder_Right = 0;
int balance_up = 0;
int velocity = 0;
int turn_out = 0;
short gx, gy, gz = 0;
float pitch, roll, yaw = 0;
int PWMA, PWMB = 0;
float tempFloat;
int8_t tempInt8[2];
uint8_t *f_ptr;
uint8_t uart1_buf[8];
uint8_t color_flag = 0;
uint8_t tempEncoder = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  fezui_init();
  /* PRESETS BEGIN */
  Flash_Recovery();
  /*
  if (HAL_GPIO_ReadPin(BOOT_GPIO_Port, BOOT_Pin))
  {
    //while(1)
    while (UI_Flag)
    {
       u8g2_ClearBuffer(&u8g2);
       if(OK_BUTTON)
       u8g2_DrawStr(&u8g2,5,ITEM_HEIGHT*1,"OK");
       if(BACK_BUTTON)
       u8g2_DrawStr(&u8g2,5,ITEM_HEIGHT*2,"BACK");
       if(UP_BUTTON)
       u8g2_DrawStr(&u8g2,5,ITEM_HEIGHT*3,"UP");
       if(DOWN_BUTTON)
       u8g2_DrawStr(&u8g2,5,ITEM_HEIGHT*4,"DOWN");
       u8g2_SendBuffer(&u8g2);
      UI_Update();
      UI_Render();
      HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    }
    Flash_Save();
  }
  else
  {
    HAL_UART_Receive_IT(&huart1, uart1_buf, 1);
    while (uart1_buf[0] != 0x10)
    {
      u8g2_ClearBuffer(&u8g2);
      u8g2_DrawStr(&u8g2, 10, 40, "Waiting for starting sign");
      u8g2_SendBuffer(&u8g2);
      HAL_UART_Receive_IT(&huart1, uart1_buf, 1);
    }

  }
  */
  //Turn.pGain=15;
  //Turn.dGain=25;
  //motor_pid_l.pGain=20;
  //motor_pid_l.iGain=10;
  //motor_pid_r.pGain=20;
  //motor_pid_r.iGain=10;
  /* PRESETS END */

  HAL_UART_Receive_IT(&huart2, &USART_RX_BYTE, 1);
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); //ï¿½ï¿½NVICï¿½Ð¶Ï¿ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð¹Ø±ï¿½EXTI12ï¿½Ð¶ï¿½
  //MPU6050_Init(); //ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½mpu6050
//  __HAL_TIM_CLEAR_FLAG(&htim1,TIM_FLAG_UPDATE);
  HAL_TIM_Base_Start_IT(&htim6);

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //ï¿½ï¿½ï¿½ï¿½TIM2ï¿½ï¿½PWM
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL); //ï¿½ï¿½ï¿½ï¿½TIM3 4ï¿½Ä±ï¿½ï¿½ï¿½ï¿½ï¿½Ä£Ê½
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);

  //HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);//ï¿½ï¿½NVICï¿½Ð¶Ï¿ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½EXTI12ï¿½Ð¶ï¿½


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
    if(USART_RX_FLG)
    {
      //sscanf(USART_RX_BUF,"%f",&bias_error);
      if(USART_RX_CNT==6)
      {
        //HAL_IWDG_Refresh(&hiwdg);
        //f_ptr = (unsigned char*)&bias_error;
        sscanf((const char *)USART_RX_BUF,"\x2c\x12%c%c%c\x5b",tempInt8,tempInt8+1,&color_flag);
        switch(color_flag)
        {
          case 2:
            bias_error=(float)tempInt8[0];
            break;
          default:
            bias_error=(float)tempInt8[1];
            break;
        }
      }

      //sprintf(USART_RX_STR,"%f",bias_error);
      //strcpy(USART_RX_STR,USART_RX_BUF);
      USART_RX_CNT=0;
      USART_RX_FLG=0;
    }
//    tempEncoder=(motor_l.Encoder+motor_r.Encoder)/2;
    HAL_UART_Transmit(&huart1,&tempEncoder,1,0xff);
    fezui_timer_handler();
   // Give_Motor_PWM(4000, 4000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
