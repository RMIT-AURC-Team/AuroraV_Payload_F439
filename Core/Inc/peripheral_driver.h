/*
 * peripheral_driver.h
 *
 *  Created on: May 22, 2024
 *      Author: aidencontini
 */

#ifndef INC_PERIPHERAL_DRIVER_H_
#define INC_PERIPHERAL_DRIVER_H_

#include "main.h"
#include "uart_transmit.h"
#include "data_management.h"
#include "data_output_spi.h"
#include "accelerometer.h"

void heartbeatUART(UART_HandleTypeDef *huart);
void eraseFlashSPI(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, uint8_t flashNo);
void readFlashToUART(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, uint8_t flashNo);


#endif /* INC_PERIPHERAL_DRIVER_H_ */
