/*
 * data_read.c
 *
 *  Created on: Mar 24, 2024
 *      Author: aidencontini
 */

#include "data_read_spi.h"

uint8_t check_status_register(SPI_HandleTypeDef *hspi, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);
	return spi_sendOp_readByte(&FLASH_READSR1, hspi, config.GPIOx, config.GPIO_Pin_CS);
}

uint32_t find_next_blank_page(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, GPIO_PinState *end_of_flash_ptr, uint8_t flashNo) {
	uint8_t firstBytes = 4;
	GPIO_Config config = getGPIOConfig(flashNo);

	uint8_t page_start[firstBytes];
	for (int i = 0; i < firstBytes; i++) {
		page_start[i] = 0;
	}

	uint32_t page_start_concat = (page_start[0] << 24) | (page_start[1] << 16) | (page_start[2] << 8) | page_start[3];
	uint32_t temp = 0;
	uint32_t page_address = temp;

	while ((page_start_concat != 0xFFFFFFFF) || (temp == NUM_OF_PAGES)) {
		page_address = temp;

		uint8_t busy = 0x01;
		while(busy) {
			busy = (check_status_register(hspi, flashNo) & 0x01);	// Check if there is a write in progress
		}

		spi_read_data(&FLASH_READEN, firstBytes, page_start, hspi, page_address, config.GPIOx, config.GPIO_Pin_CS);

		page_start_concat = (page_start[0] << 24) | (page_start[1] << 16) | (page_start[2] << 8) | page_start[3];
		temp += PAGE_SIZE;
	}

	if (page_address == (NUM_OF_PAGES * PAGE_SIZE)) {
		*end_of_flash_ptr = GPIO_PIN_RESET;
	}
	return page_address;
}

void read_page_spi(uint8_t data_read[PAGE_SIZE], SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);

	// Read the status register to ensure no write is currently in progress
	uint8_t busy = 0x01;
	while(busy) {
		busy = (check_status_register(hspi, flashNo) & 0x01);	// Check if there is a write in progress
	}

	spi_read_data(&FLASH_READEN, PAGE_SIZE, data_read, hspi, addr, config.GPIOx, config.GPIO_Pin_CS);
}

void read_manufacturer_id(uint8_t manu[2], SPI_HandleTypeDef *hspi, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);

	// Read the status register to ensure no write is currently in progress
	uint8_t busy = 0x01;
	while(busy) {
		busy = (check_status_register(hspi, flashNo) & 0x01);	// Check if there is a write in progress
	}

	uint8_t addr = 0x00;

	// Read the entire contents of a page starting from the given address
	spi_read_data(&FLASH_READ_MANU, 2, manu, hspi, addr, config.GPIOx, config.GPIO_Pin_CS);
}
