/*
 * data_read.h
 *
 *  Created on: Mar 24, 2024
 *      Author: aidencontini
 */

#ifndef INC_DATA_READ_SPI_H_
#define INC_DATA_READ_SPI_H_
#include "data_management.h"

uint8_t check_status_register(SPI_HandleTypeDef *hspi, uint8_t flashNo);
void read_page_spi(uint8_t data_read[PAGE_SIZE], SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t flashNo);
uint32_t find_next_blank_page(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, GPIO_PinState *end_of_flash_ptr, uint8_t flashNo);
void read_manufacturer_id(uint8_t manu[2], SPI_HandleTypeDef *hspi, uint8_t flashNo);


#endif /* INC_DATA_READ_SPI_H_ */
