/*
 * accelerometer.c
 *
 *  Created on: Apr 1, 2024
 *      Author: aidencontini
 */

#include "accelerometer.h"

const uint8_t H3LIS331HH_ADDR = 0x19 << 1;		// 7bit device address
uint8_t CTRL_REG1 = 0x20;
uint8_t CTRL_REG4 = 0x23;
uint8_t OUT_X_L = 0x28;
uint8_t OUT_X_H = 0x29;
uint8_t OUT_Y_L = 0x2A;
uint8_t OUT_Y_H = 0x2B;
uint8_t OUT_Z_L = 0x2C;
uint8_t OUT_Z_H = 0x2D;

HAL_StatusTypeDef init_accel(I2C_HandleTypeDef* hi2c) {
	HAL_StatusTypeDef ret;
	uint8_t control_reg = 0x00;
	ret = HAL_I2C_Master_Transmit(hi2c, H3LIS331HH_ADDR, &CTRL_REG1, 1, 100);
	ret = HAL_I2C_Master_Receive(hi2c, H3LIS331HH_ADDR, &control_reg, 1, 100);

	control_reg = control_reg | MASK_1kHz;
	uint8_t data[2] = {CTRL_REG1, control_reg};
	ret = HAL_I2C_Master_Transmit(hi2c, H3LIS331HH_ADDR, data, 2, 100);

	ret = HAL_I2C_Master_Transmit(hi2c, H3LIS331HH_ADDR, &CTRL_REG4, 1, 100);
	ret = HAL_I2C_Master_Receive(hi2c, H3LIS331HH_ADDR, &control_reg, 1, 100);

	control_reg = control_reg | MASK_24G;
	uint8_t data2[2] = {CTRL_REG4, control_reg};
	ret = HAL_I2C_Master_Transmit(hi2c, H3LIS331HH_ADDR, data2, 2, 100);

	return ret;
}

HAL_StatusTypeDef readAccelerometer(uint8_t values[6], I2C_HandleTypeDef* hi2c) {
	HAL_StatusTypeDef ret;
	uint8_t buf_x[2];
	uint8_t buf_y[2];
	uint8_t buf_z[2];

	// Read X-Axis
	ret = HAL_I2C_Master_Transmit(hi2c, H3LIS331HH_ADDR, &OUT_X_L, 1, 100);
	ret = HAL_I2C_Master_Receive(hi2c, H3LIS331HH_ADDR, &buf_x[0], 1, 100);
	ret = HAL_I2C_Master_Transmit(hi2c, H3LIS331HH_ADDR, &OUT_X_H, 1, 100);
	ret = HAL_I2C_Master_Receive(hi2c, H3LIS331HH_ADDR, &buf_x[1], 1, 100);

	// Read Y-Axis
	ret = HAL_I2C_Master_Transmit(hi2c, H3LIS331HH_ADDR, &OUT_Y_L, 1, 100);
	ret = HAL_I2C_Master_Receive(hi2c, H3LIS331HH_ADDR, &buf_y[0], 1, 100);
	ret = HAL_I2C_Master_Transmit(hi2c, H3LIS331HH_ADDR, &OUT_Y_H, 1, 100);
	ret = HAL_I2C_Master_Receive(hi2c, H3LIS331HH_ADDR, &buf_y[1], 1, 100);

	// Read Z-Axis
	ret = HAL_I2C_Master_Transmit(hi2c, H3LIS331HH_ADDR, &OUT_Z_L, 1, 100);
	ret = HAL_I2C_Master_Receive(hi2c, H3LIS331HH_ADDR, &buf_z[0], 1, 100);
	ret = HAL_I2C_Master_Transmit(hi2c, H3LIS331HH_ADDR, &OUT_Z_H, 1, 100);
	ret = HAL_I2C_Master_Receive(hi2c, H3LIS331HH_ADDR, &buf_z[1], 1, 100);


	for (int i = 0; i < 2; i++) {
		values[i] = buf_x[i];
		values[i+2] = buf_y[i];
		values[i+4] = buf_z[i];
	}

	return ret;
}

uint8_t readAccel_ctrl_rg1(I2C_HandleTypeDef* hi2c) {
	uint8_t ctrl = 0x00;

	HAL_I2C_Master_Transmit(hi2c, H3LIS331HH_ADDR, &CTRL_REG1, 1, 100);
	HAL_I2C_Master_Receive(hi2c, H3LIS331HH_ADDR, &ctrl, 1, 100);

	return ctrl;
}
