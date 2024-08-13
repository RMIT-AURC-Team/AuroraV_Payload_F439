/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
CAN_HandleTypeDef hcan2;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi2_tx;

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
I2C_HandleTypeDef *i2c_accel;
I2C_HandleTypeDef *i2c_bme280;

uint8_t UARTRxData[2];
Flag_State uart2_rec_flag;
uint32_t CAN_TxMailbox;
CAN_RxHeaderTypeDef CAN_RxHeader;
uint8_t CAN_RxData[CAN_PL_LGTH];
Flag_State CAN_RX_Flag;
Flag_State CAN_First_Msg;

Flag_State tim6_overflow_flag;
Flag_State tim7_overflow_flag;
uint8_t data_buffer_tx[2][PAGE_SIZE];
uint8_t buffer_tracker;
uint8_t accel_data[6];
uint8_t bme280_data_0[6];
uint8_t bme280_data_1[6];
uint8_t buffer_ref;
uint32_t next_blank_page;
uint16_t byte_tracker;
GPIO_PinState end_of_flash;
GPIO_PinState *end_of_flash_ptr;

GPIO_Config led_orange;
GPIO_Config led_green;
GPIO_Config status_led;
GPIO_Config cs_spi1;
GPIO_Config wp_spi1;
GPIO_Config cs_spi2;
GPIO_Config wp_spi2;
volatile GPIO_Config jmp_flight;
Flag_State rtc_reset;

uint8_t sysStatus;
Flight_State flight_state;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM6_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI2_Init(void);
static void MX_CAN2_Init(void);
static void MX_TIM7_Init(void);
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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_I2C2_Init();
  MX_SPI2_Init();
  MX_CAN2_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  systemInit();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  // If the flight jumper has been removed (GPIO rising edge), reset the time to 0.
	  if(rtc_reset == FLAG_SET) {
		  initialise_rtc_default(&hrtc);
		  rtc_reset = FLAG_RESET;
	  }

	  // Handle UART receive flag
	  if(uart2_rec_flag == FLAG_SET) {
		  handleUART();
		  uart2_rec_flag = FLAG_RESET;
	  }

	  // Handle CAN receive flag
	  if(CAN_RX_Flag == FLAG_SET) {
		  if((CAN_First_Msg == FLAG_SET) && (flight_state == GROUND)) {
			  flight_state = LOADED;
		  }

		  handleCAN();
		  HAL_GPIO_TogglePin(led_green.GPIOx, led_green.GPIO_Pin);
		  CAN_RX_Flag = FLAG_RESET;
	  }

	  if (tim7_overflow_flag == FLAG_SET) {
		  HAL_GPIO_TogglePin(led_orange.GPIOx, led_orange.GPIO_Pin);		// Toggle LED
		  sysStatus = systemStatus(&hspi1, &hspi2, i2c_bme280, i2c_accel);

		  // TODO
//		  if(sysStatus == 0x00) {
//			  HAL_GPIO_WritePin(status_led.GPIOx, status_led.GPIO_Pin, GPIO_PIN_SET);	// Turn LED off
//		  }

		  tim7_overflow_flag = FLAG_RESET;
	  }

	  // Handle Timer 6 overflow flag
	  if(tim6_overflow_flag == FLAG_SET) {
		  readAllSensors(i2c_accel, i2c_bme280, &hrtc);
		  tim6_overflow_flag = FLAG_RESET;
	  }

	  // Write data to flash when buffer is full
	  if(byte_tracker > (PAGE_SIZE - READ_SIZE)) {
		  GPIO_PinState flight_mode = HAL_GPIO_ReadPin(jmp_flight.GPIOx, jmp_flight.GPIO_Pin);
		  if((flight_mode & !(end_of_flash)) == FLAG_SET) {
			  HAL_GPIO_WritePin(led_green.GPIOx, led_green.GPIO_Pin, GPIO_PIN_SET);		// Toggle LED when writing data

			  write_data_spi_dma(data_buffer_tx[buffer_tracker], &hspi1, next_blank_page, cs_spi1);
			  write_data_spi_dma(data_buffer_tx[buffer_tracker], &hspi2, next_blank_page, cs_spi2);
			  next_blank_page += PAGE_SIZE;

			  HAL_GPIO_WritePin(led_green.GPIOx, led_green.GPIO_Pin, GPIO_PIN_RESET);		// Toggle LED when writing data
		  }

		// Reset the buffer trackers
		buffer_tracker = buffer_tracker ^ 0x01;
		byte_tracker = 0;
	  }

	  if(next_blank_page == (NUM_OF_PAGES*PAGE_SIZE)) {
		  // TODO - Error Handle if SPI1 not found
		  next_blank_page = find_next_blank_page(&hspi1, &end_of_flash, cs_spi1);
	  }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
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

/**
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 24;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_2TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_11TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */

  /* USER CODE END CAN2_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 20999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 80;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 20999;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 4000;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SPI2_WP_Pin|SPI2_CS_Pin|SPI1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI1_WP_GPIO_Port, SPI1_WP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED3_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SPI2_WP_Pin */
  GPIO_InitStruct.Pin = SPI2_WP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI2_WP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI2_CS_Pin SPI1_CS_Pin */
  GPIO_InitStruct.Pin = SPI2_CS_Pin|SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI1_WP_Pin */
  GPIO_InitStruct.Pin = SPI1_WP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI1_WP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Flight_JMP_Pin */
  GPIO_InitStruct.Pin = Flight_JMP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Flight_JMP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED3_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED3_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void clean_data_buffer(uint16_t array_size, uint8_t data_array[array_size]) {
    for (int i = 0; i < array_size; ++i) {
    	data_array[i] = 0xFF;  // Initialize each element to 0xFF
    }
}

void systemInit() {
	i2c_accel = &hi2c2;
	i2c_bme280 = &hi2c1;

	configureCAN();
	CAN_TxMailbox = 0;
	CAN_First_Msg = FLAG_RESET;
	clean_data_buffer(8, CAN_RxData);

	gpio_set_config();
	HAL_GPIO_WritePin(led_orange.GPIOx, led_orange.GPIO_Pin, GPIO_PIN_RESET);	// Turn LED off
	HAL_GPIO_WritePin(status_led.GPIOx, status_led.GPIO_Pin, GPIO_PIN_RESET);	// Turn LED off
	HAL_GPIO_WritePin(cs_spi1.GPIOx, cs_spi1.GPIO_Pin, GPIO_PIN_SET);		// SET SPI CS High to disable bus 1
	HAL_GPIO_WritePin(cs_spi2.GPIOx, cs_spi2.GPIO_Pin, GPIO_PIN_SET);		// SET SPI CS High to disable bus 2

	// Clean the data buffer and set all values to 0xFF
	clean_data_buffer(PAGE_SIZE, data_buffer_tx[0]);
	clean_data_buffer(PAGE_SIZE, data_buffer_tx[1]);

	for (uint8_t i = 0; i < 6; i++) {
		accel_data[i] = 0x00;
		bme280_data_0[i] = 0x00;
		bme280_data_1[i] = 0x00;
	}

	// Initialise the peripherals
	init_accel(i2c_accel);
	init_bme280(i2c_bme280, 0);
	init_bme280(i2c_bme280, 1);
	software_reset(&hspi1, cs_spi1);
	software_reset(&hspi2, cs_spi2);

	int next_blank_page0 = find_next_blank_page(&hspi1, &end_of_flash, cs_spi1);
	int next_blank_page1 = find_next_blank_page(&hspi2, &end_of_flash, cs_spi2);

	// Assign the value of next_blank_page to the larger of next_blank_page0 and next_blank_page1
	next_blank_page = (next_blank_page0 > next_blank_page1) ? next_blank_page0 : next_blank_page1;

	buffer_ref = 0;
	byte_tracker = 0;
	end_of_flash = FLAG_RESET;
	uart2_rec_flag = FLAG_RESET;
	CAN_RX_Flag = FLAG_RESET;
	tim6_overflow_flag = FLAG_RESET;
	tim7_overflow_flag = FLAG_RESET;
	flight_state = GROUND;
	rtc_reset = FLAG_RESET;

	sysStatus = systemStatus(&hspi1, &hspi2, i2c_bme280, i2c_accel);

	send_uart_hex(&huart2, sysStatus);

	// Initiate clocks, interrupts, CAN and DMA
	HAL_UART_Receive_IT(&huart2, UARTRxData, 2);
	HAL_CAN_Start(&hcan2);
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
	initialise_rtc_default(&hrtc);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);
}

void gpio_set_config() {
	// Set LED gpio
	led_orange = create_GPIO_Config(GPIOB, GPIO_PIN_14);	// Orange LED (Heartbeat LED)
	led_green = create_GPIO_Config(GPIOB, GPIO_PIN_7);		// Green LED (Hard Drive LED)
//	status_led = create_GPIO_Config(GPIOB, GPIO_PIN_7);		//	TODO

	// SPI Flash 0 CS and WP
	cs_spi1 = create_GPIO_Config(GPIOC, GPIO_PIN_4);		// Change for SRAD
	wp_spi1 = create_GPIO_Config(GPIOA, GPIO_PIN_4);

	// SPI Flash 1 CS and WP
	cs_spi2 = create_GPIO_Config(GPIOC, GPIO_PIN_1);		// Change for SRAD
	wp_spi2 = create_GPIO_Config(GPIOC, GPIO_PIN_0);

	// Flight Jumper GPIO Input
	jmp_flight = create_GPIO_Config(GPIOB, GPIO_PIN_1);
}

void handleCAN() {
	// Process "clock-sync". Change flight-state to "rocket_loaded"
	if(CAN_RxHeader.StdId == CLK_SYNC_ID) {
		flight_state = LOADED;
		uint8_t TxData[1] = {0x00};
		sendCAN_TxMessage(&hcan2, 1, TxData, &CAN_TxMailbox, CLK_SYNC_ID);
	}

	// Transmit values from BME280_0
	else if(CAN_RxHeader.StdId == TX_BME280_0) {
		sendCAN_TxMessage(&hcan2, 6, bme280_data_0, &CAN_TxMailbox, TX_BME280_0);
	}

	// Transmit values from BME280_1
	else if(CAN_RxHeader.StdId == TX_BME280_1) {
		sendCAN_TxMessage(&hcan2, 6, bme280_data_1, &CAN_TxMailbox, TX_BME280_1);
	}

	// Transmit values from accelerometer
	else if(CAN_RxHeader.StdId == TX_ACCEL) {
		sendCAN_TxMessage(&hcan2, 6, accel_data, &CAN_TxMailbox, TX_ACCEL);
	}

	// Transmit values from accelerometer
	else if(CAN_RxHeader.StdId == DUMMY_ID) {
		send_uart_hex(&huart2, sysStatus);
	}
}

void handleUART() {
	UART_HandleTypeDef *huart = &huart2;

	// Send Heartbeat to UART (data_rx[0] = "h")
	if (UARTRxData[0] == 0x68) {
		heartbeatUART(huart);
		send_uart_hex(huart, sysStatus);
	}

/***************************** SPI Flash ************************************************/
	// Erase specified flash chip (data_rx[0]  = "e")
	else if (UARTRxData[0] == 0x65) {
		if(decodeASCII(UARTRxData[1]) == 0) {
			eraseFlashSPI(&hspi1, huart, cs_spi1);
		} else if (decodeASCII(UARTRxData[1]) == 1) {
			eraseFlashSPI(&hspi2, huart, cs_spi2);
		}
	}

	// Read data from specified flash chip (data_rx[0] = "r")
	else if (UARTRxData[0] == 0x72) {
		if(decodeASCII(UARTRxData[1]) == 0) {
			readFlashToUART(&hspi1, huart, cs_spi1);
		} else if (decodeASCII(UARTRxData[1]) == 1) {
			readFlashToUART(&hspi2, huart, cs_spi2);
		}
	}

	// Read Manufacturer over SPI (data_rx[0] = "m")
	else if (UARTRxData[0] == 0x6d) {
		if(decodeASCII(UARTRxData[1]) == 0) {
			readFlashManuSPI(&hspi1, huart, cs_spi1);
		} else if (decodeASCII(UARTRxData[1]) == 1) {
			readFlashManuSPI(&hspi2, huart, cs_spi2);
		}
	}

	// Write a page over SPI (data_rx[0] = "w")
	else if (UARTRxData[0] == 0x77) {
		if(decodeASCII(UARTRxData[1]) == 0) {
			writePageSPI_W(&hspi1, huart, cs_spi1);
		} else if (decodeASCII(UARTRxData[1]) == 1) {
			writePageSPI_W(&hspi2, huart, cs_spi2);
		}
	}

	// Software reset flash chip over SPI (data_rx[0] = "x")
	else if (UARTRxData[0] == 0x78) {
		if(decodeASCII(UARTRxData[1]) == 0) {
			resetSPIFlash(&hspi1, huart, cs_spi1);
		} else if (decodeASCII(UARTRxData[1]) == 1) {
			resetSPIFlash(&hspi2, huart, cs_spi2);
		}
	}

/*********************************** I2C Accelerometer ***********************************/
	// Read Accelerometer WhoAmI (data_rx[0] = "c")
	else if (UARTRxData[0] == 0x63) {
		checkAccelWhoAmI(i2c_accel, huart);
	}

	// Read the accelerometer and print to the UART[0] (data_rx [0] = "a")
	else if (UARTRxData[0] == 0x61) {
		accelToUART(huart);
	}

/********************************** I2C BME280 *******************************************/
	// Read the temp sensor ID and print to the UART[0] (data_rx [0]= "b")
	else if (UARTRxData[0] == 0x62) {
		readTempSensorID(i2c_bme280, huart, decodeASCII(UARTRxData[1]));
	}

	// Read the temp sensor calibration registers and print to the UART (data_rx[0] = "p")
	else if (UARTRxData[0] == 0x70) {
		readTempCalibration(i2c_bme280, huart, decodeASCII(UARTRxData[1]));
	}

	// Read the temp sensor and print to the UART[0] (data_rx [0]= "t")
	else if (UARTRxData[0] == 0x74) {
		readTempSensor(huart, decodeASCII(UARTRxData[1]));
	}

	/********************************** CAN Bus *******************************************/
	// Send the second byte received over the CAN bus as a payload with id 0x700 (data_rx [0]= "n")
	else if (UARTRxData[0] == 0x6E) {
		uint8_t TxData[1] = {UARTRxData[1]};
		sendCAN_TxMessage(&hcan2, 1, TxData, &CAN_TxMailbox, DUMMY_ID);
	}

	UARTRxData[0] = 0x00;
	UARTRxData[1] = 0x00;
}

uint8_t decodeASCII(uint8_t asciiVal) {
	int returnVal = -1;
	if ((asciiVal >= 48) && (asciiVal <= 57)) {
		returnVal = asciiVal - 48;
	}
	return returnVal;
}

uint8_t combine_system_status() {
    // Mask status to use only bits 4:0
    uint8_t masked_status = sysStatus & 0x1F; 			// 0001 1111b

    // Mask flight_state to use only bits 2:0
    uint8_t masked_flight_state = flight_state & 0x07; 	// 0000 0111b
    // Shift flight_state to the correct position (bits 7:5)
    uint8_t shifted_flight_state = masked_flight_state << 5;

    // Combine the masked_status and shifted_flight_state
    uint8_t combined_value = masked_status | shifted_flight_state;

    return combined_value;
}

void configureCAN() {
	CAN_FilterTypeDef canFilterConfig;

	canFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
	canFilterConfig.FilterBank = 10;
	canFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	canFilterConfig.FilterIdHigh = 0x500 << 5;				// Filter only messages with ID 1X1XXXXXXXXb
	canFilterConfig.FilterIdLow = 0x0000;
	canFilterConfig.FilterMaskIdHigh = 0x500 << 5;
	canFilterConfig.FilterMaskIdLow = 0x0000;
	canFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	canFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	canFilterConfig.SlaveStartFilterBank = 0;

	HAL_CAN_ConfigFilter(&hcan2, &canFilterConfig);
}


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
