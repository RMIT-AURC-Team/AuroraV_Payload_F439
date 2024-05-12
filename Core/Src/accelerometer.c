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
	uint8_t buf_x[2];
	uint8_t buf_y[2];
	uint8_t buf_z[2];

	// Read X-Axis
	buf_x[0] = i2c_read_reg(hi2c, H3LIS331DL_ADDR, &OUT_X_L);
	buf_x[1] = i2c_read_reg(hi2c, H3LIS331DL_ADDR, &OUT_X_H);

	// Read Y-Axis
	buf_y[0] = i2c_read_reg(hi2c, H3LIS331DL_ADDR, &OUT_Y_L);
	buf_y[1] = i2c_read_reg(hi2c, H3LIS331DL_ADDR, &OUT_Y_H);

	// Read Z-Axis
	buf_z[0] = i2c_read_reg(hi2c, H3LIS331DL_ADDR, &OUT_Z_L);
	buf_z[1] = i2c_read_reg(hi2c, H3LIS331DL_ADDR, &OUT_Z_H);

	for (int i = 0; i < 2; i++) {
		values[i] = buf_x[i];
		values[i+2] = buf_y[i];
		values[i+4] = buf_z[i];
	}
}

uint8_t readAccel_whoami(I2C_HandleTypeDef* hi2c) {
	return i2c_read_reg(hi2c, H3LIS331DL_ADDR, &WHOAMI);
}
