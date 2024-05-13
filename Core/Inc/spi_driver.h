/*
 * spi_driver.h
 *
 *  Created on: May 13, 2024
 *      Author: aidencontini
 */

#ifndef INC_SPI_DRIVER_H_
#define INC_SPI_DRIVER_H_

#include "stm32f4xx_hal.h"

uint8_t spi_sendOp_readByte(uint8_t opcode, SPI_HandleTypeDef *hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS);
void perform_operation(uint8_t opcode, SPI_HandleTypeDef *hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS);
void spi_read_data(uint8_t opcode, uint16_t data_size, uint8_t data_read[data_size], SPI_HandleTypeDef *hspi, uint32_t addr, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CS);

#endif /* INC_SPI_DRIVER_H_ */
