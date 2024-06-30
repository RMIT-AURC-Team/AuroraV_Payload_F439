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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "data_management.h"
#include "rtc.h"
#include "uart_transmit.h"
#include "data_read_spi.h"
#include "data_output_spi.h"
#include "accelerometer.h"
#include "bme280.h"
#include "peripheral_driver.h"
#include "gpio_struct.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern uint8_t uart2_rec_flag;
extern uint8_t tim6_overflow_flag;
extern uint8_t buffer_tracker;
extern uint8_t data_buffer_tx[2][PAGE_SIZE];
extern uint8_t buffer_ref;
extern uint16_t byte_tracker;
extern uint8_t accel_data[6];
extern uint8_t bme280_data_1[6];
extern uint8_t bme280_data_2[6];

extern uint8_t UARTRxData[2];
extern uint32_t next_blank_page;

extern GPIO_PinState end_of_flash;
extern GPIO_PinState *end_of_flash_ptr;
extern GPIO_Config led_orange;
extern GPIO_Config led_green;
extern GPIO_Config cs_spi1;
extern GPIO_Config wp_spi1;
extern GPIO_Config cs_spi2;
extern GPIO_Config wp_spi2;
extern GPIO_Config jmp_flight;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI2_WP_Pin GPIO_PIN_0
#define SPI2_WP_GPIO_Port GPIOC
#define SPI2_CS_Pin GPIO_PIN_1
#define SPI2_CS_GPIO_Port GPIOC
#define SPI1_WP_Pin GPIO_PIN_4
#define SPI1_WP_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOC
#define JMP_Flight_Pin GPIO_PIN_1
#define JMP_Flight_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_14
#define LED3_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
void clean_data_buffer(uint16_t array_size, uint8_t data_array[array_size]);
void systemInit();
void gpio_set_config();
void handleUART();
uint8_t decodeASCII(uint8_t asciiVal);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
