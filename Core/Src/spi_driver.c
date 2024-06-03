/*
 * spi_driver.c
 *
 *  Created on: May 13, 2024
 *      Author: aidencontini
 */

#ifndef SRC_SPI_DRIVER_C_
#define SRC_SPI_DRIVER_C_

#include "spi_driver.h"

uint8_t spi_sendOp_readByte(uint8_t* opcode, SPI_HandleTypeDef *hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS) {
	uint8_t ret_data = 0x00;

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_RESET);	// Set the chip select pin to low to initiate SPI comms
	HAL_SPI_Transmit(hspi, opcode, 1, 100);	// Write the write enable value to the Flash to initial reading
	HAL_SPI_Receive(hspi, &ret_data, 1, 100);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_SET);

	return ret_data;
}

void perform_operation(uint8_t* opcode, SPI_HandleTypeDef *hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS) {
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_RESET);	// Set the chip select pin to low to initiate SPI comms
	HAL_SPI_Transmit(hspi, opcode, 1, 100);				// Write the opcode value
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_SET);
}


void spi_read_data(uint8_t* opcode, uint16_t data_size, uint8_t data_read[data_size], SPI_HandleTypeDef *hspi, uint32_t addr, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS) {
	// Split the 24 bit address into three 8 bit ints
	uint8_t addrL = addr & 0xFF;  // Get the low byte
	uint8_t addrM = (addr >> 8) & 0xFF;  // Get the middle byte
	uint8_t addrH = (addr >> 16) & 0xFF;  // Get the high byte

	// Read the data denoted by the given address
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, opcode, 1, 100);	// Write the read enable value

	// Transmit the 24 bit address of the page to initialise read from
	HAL_SPI_Transmit(hspi, &addrH, 1, 100);
	HAL_SPI_Transmit(hspi, &addrM, 1, 100);
	HAL_SPI_Transmit(hspi, &addrL, 1, 100);

	HAL_SPI_Receive(hspi, data_read, data_size, 100);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_SET);
}

void spi_write_data(uint8_t* opcode, uint16_t data_size, uint8_t data_write[data_size], SPI_HandleTypeDef *hspi, uint32_t addr, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS) {
	// Split the 24 bit address into three 8 bit ints
	uint8_t addrL = addr & 0xFF;  			// Get the low byte
	uint8_t addrM = (addr >> 8) & 0xFF;  	// Get the middle byte
	uint8_t addrH = (addr >> 16) & 0xFF;  	// Get the high byte

	// Send the PageWrite command
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_RESET);	// Set the chip select pin to low to initiate SPI comms
	HAL_SPI_Transmit(hspi, opcode, 1, 100);					// Write the write enable value to the Flash to initial writing

	// Transmit the 24 bit address of the page to write to
	HAL_SPI_Transmit(hspi, &addrH, 1, 100);
	HAL_SPI_Transmit(hspi, &addrM, 1, 100);
	HAL_SPI_Transmit(hspi, &addrL, 1, 100);

	// Transmit the data to be written
	HAL_SPI_Transmit(hspi, data_write, data_size, 1000);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_SET);	// Set the chip select pin to high to remove the Flash from the SPI bus
}

#endif /* SRC_SPI_DRIVER_C_ */
