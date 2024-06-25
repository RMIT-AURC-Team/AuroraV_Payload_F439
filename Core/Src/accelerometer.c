/*
 * accelerometer.c
 *
 *  Created on: Apr 1, 2024
 *      Author: aidencontini
 */

#include "accelerometer.h"

const uint8_t H3LIS331DL_ADDR 	= 0x18;		// 7bit device address
uint8_t CTRL_REG1 				= 0x20;
uint8_t CTRL_REG2 				= 0x21;
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

	// Reboot memory content
	ret = i2c_write_reg(hi2c, H3LIS331DL_ADDR, &CTRL_REG2, MASK_BOOT);

	// Write Control Register 1
	ret = i2c_write_reg(hi2c, H3LIS331DL_ADDR, &CTRL_REG1, MASK_1kHz);

	// Write Control Register 4
	ret = i2c_write_reg(hi2c, H3LIS331DL_ADDR, &CTRL_REG4, MASK_100G);
	return ret;
}

void readAccelerometer(uint8_t values[6], I2C_HandleTypeDef* hi2c) {
	for (uint8_t i = 0; i < 6; i++) {
		values[i] = 0x00;
	}

	i2c_burst_read(hi2c, H3LIS331DL_ADDR, OUT_X_L, 6, values);

	/*

	  uint8_t data[6]; // create a buffer for our incoming data

		i2c_read_reg(hi2c, &data[0], H3LIS331DL_ADDR, &OUT_X_L);
		i2c_read_reg(hi2c, &data[1], H3LIS331DL_ADDR, &OUT_X_H);

		i2c_read_reg(hi2c, &data[2], H3LIS331DL_ADDR, &OUT_Y_L);
		i2c_read_reg(hi2c, &data[3], H3LIS331DL_ADDR, &OUT_Y_H);

		i2c_read_reg(hi2c, &data[4], H3LIS331DL_ADDR, &OUT_Z_L);
		i2c_read_reg(hi2c, &data[5], H3LIS331DL_ADDR, &OUT_Z_H);

	  // The data that comes out is 12-bit data, left justified, so the lower
	  //  four bits of the data are always zero. We need to right shift by four,
	  //  then typecase the upper data to an integer type so it does a signed
	  //  right shift.
	  int16_t x = data[0] | data[1] << 8;
	  int16_t y = data[2] | data[3] << 8;
	  int16_t z = data[4] | data[5] << 8;
	  x = x >> 4;
	  y = y >> 4;
	  z = z >> 4;
	  __asm__("NOP");

//	uint8_t xL = 0x00;
//	uint8_t xH = 0x00;
//	uint8_t yL = 0x00;
//	uint8_t yH = 0x00;
//	uint8_t zL = 0x00;
//	uint8_t zH = 0x00;
//
//	i2c_read_reg(hi2c, &xL, H3LIS331DL_ADDR, &OUT_X_L);
//	i2c_read_reg(hi2c, &xH, H3LIS331DL_ADDR, &OUT_X_H);
//
//	i2c_read_reg(hi2c, &yL, H3LIS331DL_ADDR, &OUT_Y_L);
//	i2c_read_reg(hi2c, &yH, H3LIS331DL_ADDR, &OUT_Y_H);
//
//	i2c_read_reg(hi2c, &zL, H3LIS331DL_ADDR, &OUT_Z_L);
//	i2c_read_reg(hi2c, &zH, H3LIS331DL_ADDR, &OUT_Z_H);


*****/
}

uint8_t readAccel_whoami(I2C_HandleTypeDef* hi2c) {
	uint8_t data = 0x00;
	i2c_read_reg(hi2c, &data, H3LIS331DL_ADDR, &WHOAMI);

	return data;
}
