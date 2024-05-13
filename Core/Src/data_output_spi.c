/*
 * data_output.c
 *
 *  Created on: Mar 20, 2024
 *      Author: aidencontini
 */

#include "data_output_spi.h"

HAL_StatusTypeDef erase_chip_spi(SPI_HandleTypeDef *hspi) {
	HAL_StatusTypeDef status = HAL_OK;  // Initialize with success

	uint8_t busy = 0x01;
	while(busy) {
		busy = (check_status_register(hspi, 0) & 0x01);	// Check if there is a write in progress
	}

	write_enable_spi(hspi);

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);	// Set the chip select pin to low to initiate SPI comms
	status = HAL_SPI_Transmit(hspi, &FLASH_ERASE, 1, 100);	// Write the chip erase value to the FLASH to initial writing
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);

	busy = 0x01;
	while(busy) {
		busy = (check_status_register(hspi, 0) & 0x01);	// Check if there is a write in progress
	}

    // Return the status (HAL_OK for success, HAL_ERROR for failure)
    return status;
}

void erase_32k_spi(SPI_HandleTypeDef *hspi, uint32_t addr) {
//
//	uint8_t addrL = addr & 0xFF;  // Get the low byte
//	uint8_t addrM = (addr >> 8) & 0xFF;  // Get the middle byte
//	uint8_t addrH = (addr >> 16) & 0xFF;  // Get the high byte
//
//	uint8_t busy = 0x01;
//	while(busy) {
//		busy = (check_status_register(hspi) & 0x01);	// Check if there is a write in progress
//	}
//
//	write_enable_spi(hspi);
//
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);	// Set the chip select pin to low to initiate SPI comms
//	HAL_SPI_Transmit(hspi, &FLASH_32K_ERS, 1, 100);	// Write the chip erase value to the FLASH to initial writing
//	// Transmit the 24 bit address of the page to write to
//	HAL_SPI_Transmit(hspi, &addrH, 1, 100);
//	HAL_SPI_Transmit(hspi, &addrM, 1, 100);
//	HAL_SPI_Transmit(hspi, &addrL, 1, 100);
//
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	return;
}

void write_enable_spi(SPI_HandleTypeDef *hspi) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);	// Set the chip select pin to low to initiate SPI comms
	HAL_SPI_Transmit(hspi, &FLASH_WREN, 1, 100);	// Write the write enable value to the FLASH to initial writing
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
}

void write_disable_spi(SPI_HandleTypeDef *hspi) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);	// Set the chip select pin to low to initiate SPI comms
	HAL_SPI_Transmit(hspi, &FLASH_WRDIS, 1, 100);	// Write the write enable value to the FLASH to initial writing
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
}

uint8_t write_data_spi(uint8_t page[PAGE_SIZE], GPIO_PinState flight_mode, SPI_HandleTypeDef *hspi, int addr) {
	uint8_t exit = EXIT_FAILURE;

	// If in flight mode, write data out to EEPROM
	if(flight_mode == GPIO_PIN_SET) {

		uint8_t busy = 0x01;
		while(busy) {
			busy = (check_status_register(hspi, 0) & 0x01);	// Check if there is a write in progress
		}

		// Send the write enable signal
		write_enable_spi(hspi);

		// Split the 24 bit address into three 8 bit ints
		uint8_t addrL = addr & 0xFF;  // Get the low byte
		uint8_t addrM = (addr >> 8) & 0xFF;  // Get the middle byte
		uint8_t addrH = (addr >> 16) & 0xFF;  // Get the high byte

		// Send the PageWrite command
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);	// Set the chip select pin to low to initiate SPI comms
		HAL_SPI_Transmit(hspi, &FLASH_PGWR, 1, 100);	// Write the write enable value to the Flash to initial writing

		// Transmit the 24 bit address of the page to write to
		HAL_SPI_Transmit(hspi, &addrH, 1, 100);
		HAL_SPI_Transmit(hspi, &addrM, 1, 100);
		HAL_SPI_Transmit(hspi, &addrL, 1, 100);

		// Transmit the data to be written to the entire page
		HAL_SPI_Transmit(hspi, page, PAGE_SIZE, 1000);

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);	// Set the chip select pin to high to remove the Flash from the SPI bus
		exit = EXIT_SUCCESS;

	// If not in flight mode, exit
	} else {
		exit = EXIT_SUCCESS;
	}
	return exit;
}
