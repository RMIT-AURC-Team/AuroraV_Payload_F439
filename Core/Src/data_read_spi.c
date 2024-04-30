/*
 * data_read.c
 *
 *  Created on: Mar 24, 2024
 *      Author: aidencontini
 */

#include <data_read_spi.h>

uint8_t check_status_register(SPI_HandleTypeDef *hspi) {
	uint8_t status_reg = 0x00;

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);	// Set the chip select pin to low to initiate SPI comms
	HAL_SPI_Transmit(hspi, &FLASH_READSR1, 1, 100);	// Write the write enable value to the Flash to initial reading
	HAL_SPI_Receive(hspi, &status_reg, 1, 100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);

	return status_reg;
}

uint32_t find_next_blank_page(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, GPIO_PinState *end_of_flash_ptr) {
	uint8_t page_start[4] = {0, 0, 0, 0};
	uint32_t page_start_concat = (page_start[0] << 24) | (page_start[1] << 16) | (page_start[2] << 8) | page_start[3];
	uint32_t temp = 0;
	uint32_t page_address = temp;

	while ((page_start_concat != 0xFFFFFFFF) || (temp == NUM_OF_PAGES)) {
		page_address = temp;

		uint8_t busy = 0x01;
		while(busy) {
			busy = (check_status_register(hspi) & 0x01);	// Check if there is a write in progress
		}

		// Read the entire contents of a page starting from the given address
		// Split the 24 bit address into three 8 bit ints
		uint8_t addrL = page_address & 0xFF;  // Get the low byte
		uint8_t addrM = (page_address >> 8) & 0xFF;  // Get the middle byte
		uint8_t addrH = (page_address >> 16) & 0xFF;  // Get the high byte

		// Read the page denoted by the given address
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, &FLASH_READEN, 1, 100);	// Write the read enable value

		// Transmit the 24 bit address of the page to initialise read from
		HAL_SPI_Transmit(hspi, &addrH, 1, 100);
		HAL_SPI_Transmit(hspi, &addrM, 1, 100);
		HAL_SPI_Transmit(hspi, &addrL, 1, 100);

		HAL_SPI_Receive(hspi, page_start, 4, 100);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);

		page_start_concat = (page_start[0] << 24) | (page_start[1] << 16) | (page_start[2] << 8) | page_start[3];
		temp += PAGE_SIZE;
	}

	if (page_address == (NUM_OF_PAGES * PAGE_SIZE)) {
		*end_of_flash_ptr = GPIO_PIN_RESET;
	}
	return page_address;
}

void read_page_spi(uint8_t data_read[PAGE_SIZE], SPI_HandleTypeDef *hspi, uint32_t addr) {
	// Read the status register to ensure no write is currently in progress
	uint8_t busy = 0x01;
	while(busy) {
		busy = (check_status_register(hspi) & 0x01);	// Check if there is a write in progress
	}

	// Read the entire contents of a page starting from the given address
	// Split the 24 bit address into three 8 bit ints
	uint8_t addrL = addr & 0xFF;  // Get the low byte
	uint8_t addrM = (addr >> 8) & 0xFF;  // Get the middle byte
	uint8_t addrH = (addr >> 16) & 0xFF;  // Get the high byte

	// Read the page denoted by the given address
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, &FLASH_READEN, 1, 100);	// Write the read enable value

	// Transmit the 24 bit address of the page to initialise read from
	HAL_SPI_Transmit(hspi, &addrH, 1, 100);
	HAL_SPI_Transmit(hspi, &addrM, 1, 100);
	HAL_SPI_Transmit(hspi, &addrL, 1, 100);

	HAL_SPI_Receive(hspi, data_read, PAGE_SIZE, 100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
}

void read_manufacturer_id(uint8_t manu[2], SPI_HandleTypeDef *hspi) {
	// Read the status register to ensure no write is currently in progress
	uint8_t busy = 0x01;
	while(busy) {
		busy = (check_status_register(hspi) & 0x01);	// Check if there is a write in progress
	}

	// Read the entire contents of a page starting from the given address
	// Split the 24 bit address into three 8 bit ints
	uint8_t addr = 0x00;

	// Read the page denoted by the given address
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, &FLASH_READ_MANU, 1, 100);	// Write the read enable value

	// Transmit the 24 bit address of the page to initialise read from
	HAL_SPI_Transmit(hspi, &addr, 1, 100);
	HAL_SPI_Transmit(hspi, &addr, 1, 100);
	HAL_SPI_Transmit(hspi, &addr, 1, 100);

	HAL_SPI_Receive(hspi, manu, 2, 100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
}
