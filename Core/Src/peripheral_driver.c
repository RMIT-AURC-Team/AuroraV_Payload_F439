/*
 * peripheral_driver.c
 *
 *  Created on: May 22, 2024
 *      Author: aidencontini
 */

#include "peripheral_driver.h"

void heartbeatUART(UART_HandleTypeDef *huart) {
	send_uart_string(huart, "**Heartbeat**\r\n");			// Transmit the heartbeat
}

void eraseFlashSPI(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, uint8_t flashNo) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET);		// Activate the "write out" LED
	if (erase_chip_spi(hspi, flashNo) == HAL_OK) {
		send_uart_string(huart, "Successful Chip Erase\r\n");
		next_blank_page = find_next_blank_page(hspi, huart, &end_of_flash, flashNo);
	} else {
		send_uart_string(huart, "Error during chip erase. Please check the connection and try again.\r\n");
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET);	// Deactivate the "write out" LED
}

void readFlashToUART(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, uint8_t flashNo) {
	uint32_t num_of_pages = next_blank_page;
	if(num_of_pages == 0) {
		num_of_pages = PAGE_SIZE;
	}
	num_of_pages = num_of_pages/PAGE_SIZE;
	uint32_t address = 0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET);		// Activate the "write out" LED

	for (int i = 0; i < (num_of_pages); i++) {
		uint8_t page[PAGE_SIZE];
		read_page_spi(page, hspi, address, flashNo);//
		uart_transmit_page(huart, page);						// Transmit the data//
		address += PAGE_SIZE;
	}

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET);	// Deactivate the "write out" LED
}

