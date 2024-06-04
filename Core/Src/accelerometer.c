/*
 * accelerometer.c
 *
 *  Created on: Apr 1, 2024
 *      Author: aidencontini
 */

#include "accelerometer.h"

const uint8_t H3LIS331DL_ADDR 	= 0x18;		// 7bit device address
uint8_t CTRL_REG1 				= 0x20;
uint8_t CTRL_REG4 				= 0x23;
uint8_t WHOAMI 					= 0x0F;
uint8_t OUT_X_L 				= 0x28;
uint8_t OUT_X_H 				= 0x29;
uint8_t OUT_Y_L 				= 0x2A;
uint8_t OUT_Y_H 				= 0x2B;
uint8_t OUT_Z_L 				= 0x2C;
uint8_t OUT_Z_H 				= 0x2D;

HAL_StatusTypeDef init_accel(I2C_HandleTypeDef* hi2c) {
	HAL_StatusTypeDef ret;

	// Write Control Register 1
	ret = i2c_write_reg(hi2c, H3LIS331DL_ADDR, &CTRL_REG1, MASK_1kHz);

	// Write Control Register 4
	ret = i2c_write_reg(hi2c, H3LIS331DL_ADDR, &CTRL_REG4, MASK_100G);
	return ret;
}

void readAccelerometer(uint8_t values[6], I2C_HandleTypeDef* hi2c) {
	i2c_burst_read(hi2c, H3LIS331DL_ADDR, OUT_X_L, 6, values);
}

uint8_t readAccel_whoami(I2C_HandleTypeDef* hi2c) {
	uint8_t data = 0x00;
	i2c_read_reg(hi2c, &data, H3LIS331DL_ADDR, &WHOAMI);

	return data;
}
