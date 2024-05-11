/*
 * bme280.c
 *
 *  Created on: May 11, 2024
 *      Author: aidencontini
 */

#include "bme280.h"

const uint8_t BME280_ADDR 	= 0x76;		// 7bit device address
uint8_t ID_REG 				= 0xD0;
uint8_t RST_REG 			= 0xE0;
uint8_t CTRL_HUM_REG 		= 0xF2;
uint8_t CTRL_MEAS_REG 		= 0xF4;
uint8_t CONF_REG 			= 0xF5;
uint8_t OUT_PRESS_L 		= 0xF8;
uint8_t OUT_PRESS_H 		= 0xF7;
uint8_t OUT_TEMP_L			= 0xFB;
uint8_t OUT_TEMP_H 			= 0xFA;
uint8_t OUT_HUM_L 			= 0xFE;
uint8_t OUT_HUM_H 			= 0xFD;


HAL_StatusTypeDef init_bme280(I2C_HandleTypeDef* hi2c) {
	HAL_StatusTypeDef ret = HAL_OK;
//	uint8_t control_reg = 0x00;
//	ret = HAL_I2C_Master_Transmit(hi2c, BME280_ADDR, &CTRL_REG1, 1, 100);
//	ret = HAL_I2C_Master_Receive(hi2c, BME280_ADDR, &control_reg, 1, 100);
//
//	control_reg = control_reg | MASK_1kHz;
//	uint8_t data[2] = {CTRL_REG1, control_reg};
//	ret = HAL_I2C_Master_Transmit(hi2c, BME280_ADDR, data, 2, 100);
//
//	ret = HAL_I2C_Master_Transmit(hi2c, BME280_ADDR, &CTRL_REG4, 1, 100);
//	ret = HAL_I2C_Master_Receive(hi2c, BME280_ADDR, &control_reg, 1, 100);
//
//	control_reg = control_reg | MASK_100G;
//	uint8_t data2[2] = {CTRL_REG4, control_reg};
//	ret = HAL_I2C_Master_Transmit(hi2c, BME280_ADDR, data2, 2, 100);

	return ret;
}

HAL_StatusTypeDef readTempHumPres(uint8_t values[6], I2C_HandleTypeDef* hi2c) {
	HAL_StatusTypeDef ret = HAL_OK;
//	uint8_t buf_x[2];
//	uint8_t buf_y[2];
//	uint8_t buf_z[2];
//
//	// Read X-Axis
//	ret = HAL_I2C_Master_Transmit(hi2c, BME280_ADDR, &OUT_X_L, 1, 100);
//	ret = HAL_I2C_Master_Receive(hi2c, BME280_ADDR, &buf_x[0], 1, 100);
//	ret = HAL_I2C_Master_Transmit(hi2c, BME280_ADDR, &OUT_X_H, 1, 100);
//	ret = HAL_I2C_Master_Receive(hi2c, BME280_ADDR, &buf_x[1], 1, 100);
//
//	// Read Y-Axis
//	ret = HAL_I2C_Master_Transmit(hi2c, BME280_ADDR, &OUT_Y_L, 1, 100);
//	ret = HAL_I2C_Master_Receive(hi2c, BME280_ADDR, &buf_y[0], 1, 100);
//	ret = HAL_I2C_Master_Transmit(hi2c, BME280_ADDR, &OUT_Y_H, 1, 100);
//	ret = HAL_I2C_Master_Receive(hi2c, BME280_ADDR, &buf_y[1], 1, 100);
//
//	// Read Z-Axis
//	ret = HAL_I2C_Master_Transmit(hi2c, BME280_ADDR, &OUT_Z_L, 1, 100);
//	ret = HAL_I2C_Master_Receive(hi2c, BME280_ADDR, &buf_z[0], 1, 100);
//	ret = HAL_I2C_Master_Transmit(hi2c, BME280_ADDR, &OUT_Z_H, 1, 100);
//	ret = HAL_I2C_Master_Receive(hi2c, BME280_ADDR, &buf_z[1], 1, 100);
//
//
//	for (int i = 0; i < 2; i++) {
//		values[i] = buf_x[i];
//		values[i+2] = buf_y[i];
//		values[i+4] = buf_z[i];
//	}

	return ret;
}

uint8_t readBME280_id_reg(I2C_HandleTypeDef* hi2c) {
	uint8_t ctrl = 0x00;

	HAL_I2C_Master_Transmit(hi2c, (BME280_ADDR << 1) | 0x00, &ID_REG, 1, 100);
	HAL_I2C_Master_Receive(hi2c, (BME280_ADDR << 1) | 0x01, &ctrl, 1, 100);

	return ctrl;
}
