/*
 * i2c_driver.c
 *
 *  Created on: May 12, 2024
 *      Author: aidencontini
 */

#include "i2c_driver.h"

uint8_t i2c_read_reg(I2C_HandleTypeDef* hi2c, uint8_t dev_addr, uint8_t* register_addr) {
	uint8_t read = 0x00;
	HAL_I2C_Master_Transmit(hi2c, (dev_addr << 1) | 0x00, register_addr, 1, 100);
	HAL_I2C_Master_Receive(hi2c, (dev_addr << 1) | 0x01, &read, 1, 100);
	return read;
}

void i2c_burst_read(I2C_HandleTypeDef* hi2c, uint8_t dev_addr, uint8_t register_addr, uint8_t size, uint8_t data_buffer[size]) {
	// In order to read multiple bytes, it is necessary to assert the most significant bit of the sub-address field.
	uint8_t addr = register_addr | 0x80;
	HAL_I2C_Master_Transmit(hi2c, (dev_addr << 1) | 0x00, &addr, 1, 100);
	HAL_I2C_Master_Receive(hi2c, (dev_addr << 1) | 0x01, data_buffer, size, 100);
}

HAL_StatusTypeDef i2c_write_reg(I2C_HandleTypeDef* hi2c, uint8_t dev_addr, uint8_t* register_addr, uint8_t data) {
	uint8_t data_tx[2] = {*register_addr, data};
	return HAL_I2C_Master_Transmit(hi2c, (dev_addr << 1) | 0x00, data_tx, 2, 100);
}
