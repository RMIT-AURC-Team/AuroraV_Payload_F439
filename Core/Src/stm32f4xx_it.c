/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */
extern SPI_HandleTypeDef hspi1;
extern RTC_HandleTypeDef hrtc;
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern GPIO_PinState end_of_flash;
extern GPIO_PinState *end_of_flash_ptr;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

  uint8_t accel_data[6];
  readAccelerometer(accel_data, &hi2c1);

  uint32_t time = getTimestampMilliseconds(&hrtc);

  // Store the time in the buffer
  if (byte_tracker < (PAGE_SIZE - READ_SIZE)) {
    data_buffer[byte_tracker + 0] = (uint8_t) ((time >> 24) & 0xFF); // Most significant byte (MSB)
    data_buffer[byte_tracker + 1] = (uint8_t) ((time >> 16) & 0xFF);
    data_buffer[byte_tracker + 2] = (uint8_t) ((time >> 8) & 0xFF);
    data_buffer[byte_tracker + 3] = (uint8_t) (time & 0xFF); // Least significant byte (LSB)

    uint8_t array_ptr = 4;
    for (int i = 0; i < 6; i++) {
      data_buffer[byte_tracker + array_ptr] = accel_data[i];
      array_ptr += 1;
    }
    byte_tracker = byte_tracker + 10;
  }
  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	// Read the received data
	uint8_t received_data = huart2.Instance->DR;

	// Check if the received data matches the expected value (0x68 = 'h')
	if (received_data == 0x68) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET);		// Activate the "write out" LED
		send_uart_string(huart, "**Heartbeat**\r\n");			// Transmit the data
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET);	// Deactivate the "write out" LED
	}

	// Check if the received data matches the expected value (0x65 = 'e')
	else if (received_data == 0x65) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET);		// Activate the "write out" LED
		if (erase_chip_spi(&hspi1, 0) == HAL_OK) {
			send_uart_string(huart, "Successful Chip Erase\r\n");
			next_blank_page = find_next_blank_page(&hspi1, huart, &end_of_flash, 0);
		} else {
			send_uart_string(huart, "Error during chip erase. Please check the connection and try again.\r\n");
		}
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET);	// Deactivate the "write out" LED
	}

	// Check if the received data matches the expected value (0x72 = 'r')
	else if (received_data == 0x72) {
		uint32_t num_of_pages = next_blank_page;
		if(num_of_pages == 0) {
			num_of_pages = PAGE_SIZE;
		}
		num_of_pages = num_of_pages/PAGE_SIZE;
		uint32_t address = 0;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET);		// Activate the "write out" LED

		for (int i = 0; i < (num_of_pages); i++) {
			uint8_t page[PAGE_SIZE];
			read_page_spi(page, &hspi1, address, 0);//
			uart_transmit_page(huart, page);						// Transmit the data//
			address += PAGE_SIZE;
		}

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET);	// Deactivate the "write out" LED
	}

/************************************************************************************
	// Read Manufacturer over SPI (data_rx = "m")
	else if (received_data == 0x6d) {
		uint8_t manu[2] = {0, 0};
		read_manufacturer_id(manu, &hspi3);
		HAL_UART_Transmit(&huart3, manu, 2, HAL_MAX_DELAY);
	}

	// Write a page over SPI (data_rx = "w")
	else if (received_data == 0x77) {
		uint8_t data_out[PAGE_SIZE];
		for (int i = 0; i < PAGE_SIZE; i++) {
			data_out[i] = 0x77;
		}

		write_data_spi(data_out, GPIO_PIN_SET, &hspi3, next_blank_page);

		next_blank_page += PAGE_SIZE;
	}
************************************************************************************/
	// Read Accelerometer CTRL1 (data_rx = "c")
	else if (received_data == 0x63) {
		uint8_t ctrl = readAccel_whoami(&hi2c1);
		HAL_UART_Transmit(&huart2, &ctrl, 1, HAL_MAX_DELAY);
	}

	// Read the accelerometer and print to the UART (data_rx = "a")
	else if (received_data == 0x61) {
		uint8_t readings[6];
		readAccelerometer(readings, &hi2c1);
		for (int i = 0; i < 6; i++) {
			send_uart_hex(&huart2, readings[i]);
		}
	}

	// Read the temp sensor and print to the UART (data_rx = "t")
	else if (received_data == 0x74) {
		uint8_t reading = readBME280_id_reg(&hi2c2, 0);
		send_uart_hex(&huart2, reading);
	}


	HAL_UART_Receive_IT(&huart2, UARTRxData, 1);
}
/* USER CODE END 1 */
