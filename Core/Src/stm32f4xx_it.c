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
  readAllSensors(&hi2c1, &hi2c2, &hrtc);
  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	// Send Heartbeat to UART (data_rx[0] = "h")
	if (UARTRxData[0] == 0x68) {
		heartbeatUART(huart);
	}

/***************************** SPI Flash ************************************************/
	// Erase specified flash chip (data_rx[0]  = "e")
	else if (UARTRxData[0] == 0x65) {
		eraseFlashSPI(&hspi1, huart, decodeASCII(UARTRxData[1]));
	}

	// Read data from specified flash chip (data_rx[0] = "r")
	else if (UARTRxData[0] == 0x72) {
		readFlashToUART(&hspi1, huart, decodeASCII(UARTRxData[1]));
	}

	// Read Manufacturer over SPI (data_rx[0] = "m")
	else if (UARTRxData[0] == 0x6d) {
		readFlashManuSPI(&hspi1, huart, decodeASCII(UARTRxData[1]));
	}

	// Write a page over SPI (data_rx[0] = "w")
	else if (UARTRxData[0] == 0x77) {
		writePageSPI_W(&hspi1, huart, decodeASCII(UARTRxData[1]));
	}

	// Software reset flash chip over SPI (data_rx[0] = "x")
	else if (UARTRxData[0] == 0x78) {
		resetSPIFlash(&hspi1, huart, decodeASCII(UARTRxData[1]));
	}

/*********************************** I2C Accelerometer ***********************************/
	// Read Accelerometer WhoAmI (data_rx[0] = "c")
	else if (UARTRxData[0] == 0x63) {
		checkAccelWhoAmI(&hi2c1, huart);
	}

	// Read the accelerometer and print to the UART[0] (data_rx [0] = "a")
	else if (UARTRxData[0] == 0x61) {
		accelToUART(huart);
	}


/********************************** I2C BME280 *******************************************/
	// Read the temp sensor ID and print to the UART[0] (data_rx [0]= "b")
	else if (UARTRxData[0] == 0x62) {
		readTempSensorID(&hi2c2, huart, decodeASCII(UARTRxData[1]));
	}

	// Read the temp sensor calibration registers and print to the UART (data_rx[0] = "p")
	else if (UARTRxData[0] == 0x70) {
		readTempCalibration(&hi2c2, huart, decodeASCII(UARTRxData[1]));
	}

	// Read the temp sensor and print to the UART[0] (data_rx [0]= "t")
	else if (UARTRxData[0] == 0x74) {
		readTempSensor(huart, decodeASCII(UARTRxData[1]));
	}

	HAL_UART_Receive_IT(&huart2, UARTRxData, 2);
}

uint8_t decodeASCII(uint8_t asciiVal) {
	int returnVal = -1;
	if ((asciiVal >= 48) && (asciiVal <= 57)) {
		returnVal = asciiVal - 48;
	}
	return returnVal;
}
/* USER CODE END 1 */
