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
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Throttle_Pin GPIO_PIN_0
#define Throttle_GPIO_Port GPIOA
#define Debugging_indicator_Pin GPIO_PIN_4
#define Debugging_indicator_GPIO_Port GPIOA
#define A_LOW_MOSFET_Pin GPIO_PIN_7
#define A_LOW_MOSFET_GPIO_Port GPIOA
#define B_LOW_MOSFET_Pin GPIO_PIN_0
#define B_LOW_MOSFET_GPIO_Port GPIOB
#define C_LOW_MOSFET_Pin GPIO_PIN_1
#define C_LOW_MOSFET_GPIO_Port GPIOB
#define A_HIGHSIDE_MOSFET_Pin GPIO_PIN_8
#define A_HIGHSIDE_MOSFET_GPIO_Port GPIOA
#define B_HIGHSIDE_MOSFET_Pin GPIO_PIN_9
#define B_HIGHSIDE_MOSFET_GPIO_Port GPIOA
#define C_HIGHSIDE_MOSFET_Pin GPIO_PIN_10
#define C_HIGHSIDE_MOSFET_GPIO_Port GPIOA
#define HALL_A_EXTI_15_Pin GPIO_PIN_15
#define HALL_A_EXTI_15_GPIO_Port GPIOA
#define HALL_A_EXTI_15_EXTI_IRQn EXTI15_10_IRQn
#define HALL_B_EXTI_3_Pin GPIO_PIN_3
#define HALL_B_EXTI_3_GPIO_Port GPIOB
#define HALL_B_EXTI_3_EXTI_IRQn EXTI3_IRQn
#define HALL_C_EXTI_4_Pin GPIO_PIN_4
#define HALL_C_EXTI_4_GPIO_Port GPIOB
#define HALL_C_EXTI_4_EXTI_IRQn EXTI4_IRQn
#define SIM_HALL_A_Pin GPIO_PIN_5
#define SIM_HALL_A_GPIO_Port GPIOB
#define SIM_HALL_B_Pin GPIO_PIN_6
#define SIM_HALL_B_GPIO_Port GPIOB
#define SIM_HALL_C_Pin GPIO_PIN_7
#define SIM_HALL_C_GPIO_Port GPIOB
#define HALL_FREQ_PIN_Pin GPIO_PIN_8
#define HALL_FREQ_PIN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
