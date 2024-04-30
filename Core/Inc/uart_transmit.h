/*
 * uart_transmit.h
 *
 *  Created on: Mar 24, 2024
 *      Author: aidencontini
 */

#ifndef INC_UART_TRANSMIT_H_
#define INC_UART_TRANSMIT_H_

#include "stm32f4xx_hal.h"
#include "data_management.h"
#include <string.h>
#include <stdio.h>

void uart_transmit_page(UART_HandleTypeDef *huart, uint8_t page[PAGE_SIZE]);
void send_uart_string(UART_HandleTypeDef *huart, const char *str);
void send_uart_int(UART_HandleTypeDef *huart, uint32_t value);


#endif /* INC_UART_TRANSMIT_H_ */
