/*
 * uart_transmit.c
 *
 *  Created on: Mar 24, 2024
 *      Author: aidencontini
 */

#include "uart_transmit.h"
/**
 * Transmit one single page over the UART interface
 */
void uart_transmit_page(UART_HandleTypeDef *huart, uint8_t page[PAGE_SIZE]) {
	HAL_UART_Transmit(huart, page, PAGE_SIZE, HAL_MAX_DELAY);
}

void send_uart_string(UART_HandleTypeDef *huart, const char *str) {
    HAL_UART_Transmit(huart, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

void send_uart_int(UART_HandleTypeDef *huart, uint32_t value) {
	char value_str[20]; // Adjust the buffer size as needed
	snprintf(value_str, sizeof(value_str), "%lu", value);
	// Call the send_uart_string function to send the value to UART
	send_uart_string(huart, value_str);
	send_uart_string(huart, "\r\n");
}
