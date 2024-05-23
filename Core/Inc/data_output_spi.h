/*
 * data_output.h
 *
 *  Created on: Mar 20, 2024
 *      Author: aidencontini
 */

#ifndef INC_DATA_OUTPUT_SPI_H_
#define INC_DATA_OUTPUT_SPI_H_
#include "data_management.h"
#include "data_read_spi.h"

void write_enable_spi(SPI_HandleTypeDef *hspi, uint8_t flashNo);
void write_disable_spi(SPI_HandleTypeDef *hspi, uint8_t flashNo);
HAL_StatusTypeDef erase_chip_spi(SPI_HandleTypeDef *hspi, uint8_t flashNo);
void erase_32k_spi(SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t flashNo);
uint8_t write_data_spi(uint8_t page[PAGE_SIZE], GPIO_PinState flight_mode, SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t flashNo);
void software_reset(SPI_HandleTypeDef *hspi, uint8_t flashNo);

#endif /* INC_DATA_OUTPUT_SPI_H_ */
