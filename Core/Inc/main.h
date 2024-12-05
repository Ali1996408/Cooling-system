/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void debounce_buttons(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_2
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_3
#define LED2_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_4
#define LED3_GPIO_Port GPIOF
#define LED4_Pin GPIO_PIN_5
#define LED4_GPIO_Port GPIOA
#define S1_Pin GPIO_PIN_6
#define S1_GPIO_Port GPIOA
#define S1_EXTI_IRQn EXTI4_15_IRQn
#define S3_Pin GPIO_PIN_7
#define S3_GPIO_Port GPIOA
#define S3_EXTI_IRQn EXTI4_15_IRQn
#define S2_Pin GPIO_PIN_8
#define S2_GPIO_Port GPIOA
#define S2_EXTI_IRQn EXTI4_15_IRQn
#define S4_Pin GPIO_PIN_12
#define S4_GPIO_Port GPIOA
#define S4_EXTI_IRQn EXTI4_15_IRQn
#define DEBOUNCE_THRESHOLD_MS 50
#define  INACTIVITY_TIME 300000
extern volatile int button_debounce_timer[4]; 
extern volatile char buttonFlagForward; 
extern volatile char buttonFlagBackward;
extern volatile char buttonFlagCancel;
extern volatile char buttonFlagConfirm;
extern volatile uint32_t inactivity_timer ;
void TimerHandler(void);
 void handle_menu_logic(void);
void check_inactivity(void);
void toggle (void);
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
