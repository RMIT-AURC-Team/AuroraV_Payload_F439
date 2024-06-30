/*
 * i2c_driver.c
 *
 *  Created on: May 12, 2024
 *      Author: aidencontini
 */

#include "i2c_driver.h"

HAL_StatusTypeDef i2c_read_reg(I2C_HandleTypeDef* hi2c, uint8_t* data_ptr, uint8_t dev_addr, uint8_t* register_addr) {
	HAL_I2C_Master_Transmit(hi2c, (dev_addr << 1) & 0xFE, register_addr, 1, I2C_TIMEOUT_SML);
	HAL_StatusTypeDef hal_status = HAL_I2C_Master_Receive(hi2c, (dev_addr << 1) | 0x01, data_ptr, 1, I2C_TIMEOUT_SML);
	return hal_status;
}

HAL_StatusTypeDef i2c_burst_read(I2C_HandleTypeDef* hi2c, uint8_t dev_addr, uint8_t register_addr, uint8_t size, uint8_t data_buffer[size]) {
	// In order to read multiple bytes, it is necessary to assert the most significant bit of the sub-address field.
	uint8_t addr = register_addr | 0x80;
	HAL_I2C_Master_Transmit(hi2c, (dev_addr << 1) & 0xFE, &addr, 1, I2C_TIMEOUT_SML);
	HAL_StatusTypeDef hal_status =  HAL_I2C_Master_Receive(hi2c, (dev_addr << 1) | 0x01, data_buffer, size, I2C_TIMEOUT_LRG);
	return hal_status;
}

HAL_StatusTypeDef i2c_burst_read_alt(I2C_HandleTypeDef* hi2c, uint8_t dev_addr, uint8_t register_addr, uint8_t size, uint8_t data_buffer[size], uint8_t alt) {
	uint8_t addr = register_addr;
	HAL_I2C_Master_Transmit(hi2c, (dev_addr << 1) & 0xFE, &addr, 1, I2C_TIMEOUT_SML);
	HAL_StatusTypeDef hal_status =  HAL_I2C_Master_Receive(hi2c, (dev_addr << 1) | 0x01, data_buffer, size, I2C_TIMEOUT_LRG);
	return hal_status;
}

HAL_StatusTypeDef i2c_write_reg(I2C_HandleTypeDef* hi2c, uint8_t dev_addr, uint8_t* register_addr, uint8_t data) {
	uint8_t data_tx[2] = {*register_addr, data};
	HAL_StatusTypeDef hal_status = HAL_I2C_Master_Transmit(hi2c, (dev_addr << 1) & 0xFE, data_tx, 2, I2C_TIMEOUT_SML);
	return hal_status;
}
