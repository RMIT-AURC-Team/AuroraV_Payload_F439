/*
 * accelerometer.c
 *
 *  Created on: Apr 1, 2024
 *      Author: aidencontini
 */

#include "accelerometer.h"

uint8_t ADXL314_ADDR 			= 0x53;		// 7bit device address (Can also be 0x1D)
uint8_t BW_RATE_REG				= 0x2C;
uint8_t POWER_CTL_REG			= 0x2D;
uint8_t DATA_FORMAT_REG			= 0x31;
uint8_t WHOAMI 					= 0x00;
uint8_t OFFSET_REG_X			= 0x1E;
uint8_t OFFSET_REG_Y			= 0x1F;
uint8_t OFFSET_REG_Z			= 0x20;
uint8_t OUT_X_L 				= 0x32;
uint8_t OUT_X_H 				= 0x33;
uint8_t OUT_Y_L 				= 0x34;
uint8_t OUT_Y_H 				= 0x35;
uint8_t OUT_Z_L 				= 0x36;
uint8_t OUT_Z_H 				= 0x37;

HAL_StatusTypeDef init_accel(I2C_HandleTypeDef* hi2c) {
	HAL_StatusTypeDef ret;

	// Set standby mode
	ret = i2c_write_reg(hi2c, ADXL314_ADDR, &POWER_CTL_REG, 0x00);

	// Set to 200G right justified
	ret = i2c_write_reg(hi2c, ADXL314_ADDR, &DATA_FORMAT_REG, RIGHT_JUST);

	// Set to 1kHz ODR
	ret = i2c_write_reg(hi2c, ADXL314_ADDR, &BW_RATE_REG, MASK_1kHz);

	// Write the offset registers
	ret = writeOffset(hi2c);

	// Start measuring
	ret = i2c_write_reg(hi2c, ADXL314_ADDR, &POWER_CTL_REG, START_MEAS);

	return ret;
}

void readAccelerometer(uint8_t values[6], I2C_HandleTypeDef* hi2c) {
	for (uint8_t i = 0; i < 6; i++) {
		values[i] = 0x00;
	}

	i2c_burst_read(hi2c, ADXL314_ADDR, OUT_X_L, 6, values);
}

uint8_t readAccel_whoami(I2C_HandleTypeDef* hi2c) {
	uint8_t data = 0x00;
	i2c_read_reg(hi2c, &data, ADXL314_ADDR, &WHOAMI);

	return data;
}

HAL_StatusTypeDef writeOffset(I2C_HandleTypeDef* hi2c) {
	HAL_StatusTypeDef ret;

	// Define the X, Y and Z offsets
//	uint16_t offsetX = 0x0000;
//	uint16_t offsetY = 0x0000;
//	uint16_t offsetZ = 0x0000;

	uint16_t offsetX = 0xFF;
	uint16_t offsetY = 0x04;
	uint16_t offsetZ = 0x0D;

	ret = i2c_write_reg(hi2c, ADXL314_ADDR, &OFFSET_REG_X, offsetX);
	ret = i2c_write_reg(hi2c, ADXL314_ADDR, &OFFSET_REG_Y, offsetY);
	ret = i2c_write_reg(hi2c, ADXL314_ADDR, &OFFSET_REG_Z, offsetZ);

	return ret;
}


















