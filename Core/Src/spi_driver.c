/*
 * spi_driver.c
 *
 *  Created on: May 13, 2024
 *      Author: aidencontini
 */

#ifndef SRC_SPI_DRIVER_C_
#define SRC_SPI_DRIVER_C_

#include "spi_driver.h"

uint8_t spi_sendOp_readByte(uint8_t opcode, SPI_HandleTypeDef *hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS) {
	uint8_t opVal = opcode;
	uint8_t ret_data = 0x00;

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_RESET);	// Set the chip select pin to low to initiate SPI comms
	HAL_SPI_Transmit(hspi, &opVal, 1, 100);	// Write the write enable value to the Flash to initial reading
	HAL_SPI_Receive(hspi, &ret_data, 1, 100);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_SET);

	return ret_data;
}

void perform_operation(uint8_t opcode, SPI_HandleTypeDef *hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS) {
	uint8_t opVal = opcode;

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_RESET);	// Set the chip select pin to low to initiate SPI comms
	HAL_SPI_Transmit(hspi, &opVal, 1, 100);				// Write the opcode value
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_SET);
}


void spi_read_data(uint8_t opcode, uint16_t data_size, uint8_t data_read[data_size], SPI_HandleTypeDef *hspi, uint32_t addr, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS) {
	uint8_t opVal = opcode;

	// Read the entire contents of a page starting from the given address
	// Split the 24 bit address into three 8 bit ints
	uint8_t addrL = addr & 0xFF;  // Get the low byte
	uint8_t addrM = (addr >> 8) & 0xFF;  // Get the middle byte
	uint8_t addrH = (addr >> 16) & 0xFF;  // Get the high byte

	// Read the page denoted by the given address
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, &opVal, 1, 100);	// Write the read enable value

	// Transmit the 24 bit address of the page to initialise read from
	HAL_SPI_Transmit(hspi, &addrH, 1, 100);
	HAL_SPI_Transmit(hspi, &addrM, 1, 100);
	HAL_SPI_Transmit(hspi, &addrL, 1, 100);

	HAL_SPI_Receive(hspi, data_read, 4, 100);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin_CS, GPIO_PIN_SET);
}




#endif /* SRC_SPI_DRIVER_C_ */
