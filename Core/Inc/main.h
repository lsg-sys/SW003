/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TIM10_1s_per_Tick 2000
#define BufSize_Serial 256
#define PCLK2_M (HAL_RCC_GetPCLK2Freq() / 1000000u)
#define PCLK1_M (HAL_RCC_GetPCLK1Freq() / 1000000u)
#define HCLK_M (HAL_RCC_GetHCLKFreq() / 1000000u)
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define KEY_D_Pin GPIO_PIN_0
#define KEY_D_GPIO_Port GPIOA
#define KEY_D_EXTI_IRQn EXTI0_IRQn
#define KEY_C_Pin GPIO_PIN_1
#define KEY_C_GPIO_Port GPIOA
#define KEY_C_EXTI_IRQn EXTI1_IRQn
#define KEY_B_Pin GPIO_PIN_2
#define KEY_B_GPIO_Port GPIOA
#define KEY_B_EXTI_IRQn EXTI2_IRQn
#define LCD_RST_Pin GPIO_PIN_3
#define LCD_RST_GPIO_Port GPIOA
#define LCD_HWCS_Pin GPIO_PIN_4
#define LCD_HWCS_GPIO_Port GPIOA
#define LCD_SCK_Pin GPIO_PIN_5
#define LCD_SCK_GPIO_Port GPIOA
#define LCD_DC_Pin GPIO_PIN_6
#define LCD_DC_GPIO_Port GPIOA
#define LCD_MOSI_Pin GPIO_PIN_7
#define LCD_MOSI_GPIO_Port GPIOA
#define BAT_ADC_Pin GPIO_PIN_0
#define BAT_ADC_GPIO_Port GPIOB
#define BAT_ADC_EN_Pin GPIO_PIN_1
#define BAT_ADC_EN_GPIO_Port GPIOB
#define PAD_BOOT1_reserved_Pin GPIO_PIN_2
#define PAD_BOOT1_reserved_GPIO_Port GPIOB
#define MEMS_SCK_Pin GPIO_PIN_10
#define MEMS_SCK_GPIO_Port GPIOB
#define FLASH_SCK_Pin GPIO_PIN_12
#define FLASH_SCK_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_13
#define LCD_BL_GPIO_Port GPIOB
#define MEMS_MISO_Pin GPIO_PIN_14
#define MEMS_MISO_GPIO_Port GPIOB
#define MEMS_MOSI_Pin GPIO_PIN_15
#define MEMS_MOSI_GPIO_Port GPIOB
#define MEMS_SWCS_Pin GPIO_PIN_8
#define MEMS_SWCS_GPIO_Port GPIOA
#define FLASH_MISO_Pin GPIO_PIN_4
#define FLASH_MISO_GPIO_Port GPIOB
#define FLASH_MOSI_Pin GPIO_PIN_5
#define FLASH_MOSI_GPIO_Port GPIOB
#define MEMS_INT2_Pin GPIO_PIN_6
#define MEMS_INT2_GPIO_Port GPIOB
#define MEMS_INT2_EXTI_IRQn EXTI9_5_IRQn
#define MEMS_INT1_Pin GPIO_PIN_7
#define MEMS_INT1_GPIO_Port GPIOB
#define MEMS_INT1_EXTI_IRQn EXTI9_5_IRQn
#define FLASH_SWCS_Pin GPIO_PIN_8
#define FLASH_SWCS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define HW_ERROR_CODE_FLASH 1
#define HW_ERROR_CODE_MEMS 2
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
