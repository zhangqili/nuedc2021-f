/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern float Up_balance_KP;
extern float Up_balance_KD;
extern float Velocity_KP;
extern float Velocity_KI;
extern float Turn_KP;
extern float Turn_KD;


extern float Mechanical_Angle;
extern int Mechanical_velocity;
extern int turn_speed;
extern int Encoder_Left,Encoder_Right;
extern int balance_up;
extern int velocity;
extern int turn_out;
extern short gx,gy,gz;
extern float pitch,roll,yaw;
extern int PWMA,PWMB;

extern uint8_t color_flag;
extern uint32_t tick_count;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define LOG_S(state_t,state) printf("%s\t->%s\t%ld\tatL%d\n",#state_t,#state,tick_count,__LINE__);
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY0_Pin GPIO_PIN_0
#define KEY0_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_1
#define LED_GPIO_Port GPIOA
#define Encoder_Left_A_Pin GPIO_PIN_6
#define Encoder_Left_A_GPIO_Port GPIOA
#define Encoder_Left_B_Pin GPIO_PIN_7
#define Encoder_Left_B_GPIO_Port GPIOA
#define BIN2_Pin GPIO_PIN_7
#define BIN2_GPIO_Port GPIOE
#define BIN1_Pin GPIO_PIN_8
#define BIN1_GPIO_Port GPIOE
#define AIN1_Pin GPIO_PIN_9
#define AIN1_GPIO_Port GPIOE
#define AIN2_Pin GPIO_PIN_10
#define AIN2_GPIO_Port GPIOE
#define KEY1_Pin GPIO_PIN_11
#define KEY1_GPIO_Port GPIOE
#define KEY2_Pin GPIO_PIN_12
#define KEY2_GPIO_Port GPIOE
#define KEY3_Pin GPIO_PIN_13
#define KEY3_GPIO_Port GPIOE
#define KEY4_Pin GPIO_PIN_14
#define KEY4_GPIO_Port GPIOE
#define KEY5_Pin GPIO_PIN_15
#define KEY5_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
