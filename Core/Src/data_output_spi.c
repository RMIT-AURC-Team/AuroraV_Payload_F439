/*
 * data_output.c
 *
 *  Created on: Mar 20, 2024
 *      Author: aidencontini
 */

#include "data_output_spi.h"

HAL_StatusTypeDef erase_chip_spi(SPI_HandleTypeDef *hspi, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);

	uint8_t busy = 0x01;
	while(busy) {
		busy = (check_status_register(hspi, flashNo) & 0x01);	// Check if there is a write in progress
	}

	write_enable_spi(hspi, flashNo);
	perform_operation(&FLASH_ERASE, hspi, config.GPIOx, config.GPIO_Pin_CS);

	busy = 0x01;
	while(busy) {
		busy = (check_status_register(hspi, flashNo) & 0x01);	// Check if there is a write in progress
	}

	return HAL_OK;
}

void erase_32k_spi(SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);

	uint8_t busy = 0x01;
	while(busy) {
		busy = (check_status_register(hspi, flashNo) & 0x01);	// Check if there is a write in progress
	}

	write_enable_spi(hspi, flashNo);
	perform_operation(&FLASH_32K_ERS, hspi, config.GPIOx, config.GPIO_Pin_CS);
}

void write_enable_spi(SPI_HandleTypeDef *hspi, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);
	perform_operation(&FLASH_WREN, hspi, config.GPIOx, config.GPIO_Pin_CS);
}

void write_disable_spi(SPI_HandleTypeDef *hspi, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);
	perform_operation(&FLASH_WRDIS, hspi, config.GPIOx, config.GPIO_Pin_CS);
}

void software_reset(SPI_HandleTypeDef *hspi, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);
	uint8_t busy = 0x01;
	while(busy) {
		busy = (check_status_register(hspi, flashNo) & 0x01);	// Check if there is a write in progress
	}

	perform_operation(&RST_EN, hspi, config.GPIOx, config.GPIO_Pin_CS);
	perform_operation(&DEV_RST, hspi, config.GPIOx, config.GPIO_Pin_CS);
}

uint8_t write_data_spi(uint8_t page[PAGE_SIZE], GPIO_PinState flight_mode, SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);
	uint8_t exit = EXIT_FAILURE;

	// If in flight mode, write data out to flash
	if(flight_mode == GPIO_PIN_SET) {

		uint8_t busy = 0x01;
		while(busy) {
			busy = (check_status_register(hspi, flashNo) & 0x01);	// Check if there is a write in progress
		}

		// Send the write enable signal
		write_enable_spi(hspi, flashNo);
		spi_write_data(&FLASH_PGWR, PAGE_SIZE, page, hspi, addr, config.GPIOx, config.GPIO_Pin_CS);
		exit = EXIT_SUCCESS;

	// If not in flight mode, exit
	} else {
		exit = EXIT_SUCCESS;
	}
	return exit;
}
