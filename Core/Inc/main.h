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
#include "can_driver.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum {
    FLAG_RESET,
    FLAG_SET
} Flag_State;

typedef enum {
    GROUND,
    LOADED,
	ASCENT,
	DESCEND,
	RECOVERY
} Flight_State;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern uint8_t tim6_overflow_flag;
extern uint8_t tim7_overflow_flag;

extern uint8_t buffer_tracker;
extern uint8_t data_buffer_tx[2][PAGE_SIZE];
extern uint8_t buffer_ref;
extern uint16_t byte_tracker;
extern uint32_t next_blank_page;
extern GPIO_PinState end_of_flash;
extern GPIO_PinState *end_of_flash_ptr;

extern uint8_t accel_data[6];
extern uint8_t bme280_data_0[6];
extern uint8_t bme280_data_1[6];

extern uint8_t UARTRxData[2];
extern Flag_State uart2_rec_flag;

extern uint32_t CAN_TxMailbox;
extern CAN_RxHeaderTypeDef CAN_RxHeader;
extern uint8_t CAN_RxData[CAN_PL_LGTH];
extern Flag_State CAN_RX_Flag;
extern Flag_State CAN_First_Msg;

extern GPIO_Config led_orange;
extern GPIO_Config led_green;
extern GPIO_Config status_led;
extern GPIO_Config cs_spi1;
extern GPIO_Config wp_spi1;
extern GPIO_Config cs_spi2;
extern GPIO_Config wp_spi2;
extern volatile GPIO_Config jmp_flight;
extern Flag_State rtc_reset;

extern uint8_t sysStatus;
extern Flight_State flight_state;
extern uint8_t i2c_offline_cnt;

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
#define Flight_JMP_Pin GPIO_PIN_1
#define Flight_JMP_GPIO_Port GPIOB
#define Flight_JMP_EXTI_IRQn EXTI1_IRQn
#define LED3_Pin GPIO_PIN_12
#define LED3_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOB

#define I2C_OFFLINE_THRESH		3

/* USER CODE BEGIN Private defines */
void clean_data_buffer(uint16_t array_size, uint8_t data_array[array_size]);
void systemInit();
void gpio_set_config();
void configureCAN();
void handleCAN();
void handleUART();
uint8_t decodeASCII(uint8_t asciiVal);
uint8_t combine_system_status();
uint32_t find_next_blank_page_all();
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
