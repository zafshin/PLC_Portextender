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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

void HAL_UART_RxCallback();
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

/* USER CODE BEGIN EFP */
void logua(const char *string);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DO8_Pin GPIO_PIN_13
#define DO8_GPIO_Port GPIOC
#define G_LED_Pin GPIO_PIN_14
#define G_LED_GPIO_Port GPIOC
#define R_LED_Pin GPIO_PIN_15
#define R_LED_GPIO_Port GPIOC
#define DI1_Pin GPIO_PIN_0
#define DI1_GPIO_Port GPIOA
#define DI2_Pin GPIO_PIN_1
#define DI2_GPIO_Port GPIOA
#define AI1_Pin GPIO_PIN_2
#define AI1_GPIO_Port GPIOA
#define CS_flash_Pin GPIO_PIN_4
#define CS_flash_GPIO_Port GPIOA
#define reset_factory_Pin GPIO_PIN_4
#define reset_factory_GPIO_Port GPIOC
#define eth_rst_Pin GPIO_PIN_5
#define eth_rst_GPIO_Port GPIOC
#define eth_int_Pin GPIO_PIN_11
#define eth_int_GPIO_Port GPIOB
#define eth_cs_Pin GPIO_PIN_12
#define eth_cs_GPIO_Port GPIOB
#define DOPWM1_Pin GPIO_PIN_6
#define DOPWM1_GPIO_Port GPIOC
#define DOPWM2_Pin GPIO_PIN_7
#define DOPWM2_GPIO_Port GPIOC
#define DOPWM3_Pin GPIO_PIN_8
#define DOPWM3_GPIO_Port GPIOC
#define DOPWM4_Pin GPIO_PIN_9
#define DOPWM4_GPIO_Port GPIOC
#define Modbus_controller_Pin GPIO_PIN_8
#define Modbus_controller_GPIO_Port GPIOA
#define DO5_Pin GPIO_PIN_10
#define DO5_GPIO_Port GPIOC
#define DO6_Pin GPIO_PIN_11
#define DO6_GPIO_Port GPIOC
#define DO7_Pin GPIO_PIN_12
#define DO7_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
