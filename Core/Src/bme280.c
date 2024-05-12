/*
 * bme280.c
 *
 *  Created on: May 11, 2024
 *      Author: aidencontini
 */

#include "bme280.h"

const uint8_t BME280_ADDR_0	= 0x76;		// 7bit device address
const uint8_t BME280_ADDR_1 = 0x77;		// 7bit device address
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


HAL_StatusTypeDef init_bme280(I2C_HandleTypeDef* hi2c, uint8_t bme) {
	uint8_t bme_addr = 0;
	if (bme == 0) {
		bme_addr = BME280_ADDR_0;
	} else if (bme == 1) {
		bme_addr = BME280_ADDR_1;
	}

	HAL_StatusTypeDef ret;
	uint8_t control_reg = 0x00;

	// Reset the sensor
	ret =  i2c_write_reg(hi2c, bme_addr, &RST_REG, RST);

	// Set humidity control register
	control_reg = i2c_read_reg(hi2c, bme_addr, &CTRL_HUM_REG);
	control_reg = control_reg & 0xF8;
	control_reg = control_reg | CTRL_HUM_MSK;
	ret = i2c_write_reg(hi2c, bme_addr, &CTRL_HUM_REG, control_reg);

	// Set measure control register
	ret = i2c_write_reg(hi2c, bme_addr, &CTRL_MEAS_REG, CTRL_MEAS_MSK);

	// Set the config register
	ret = i2c_write_reg(hi2c, bme_addr, &CONF_REG, CONF_MSK);

	return ret;
}

void readTempHumPres(uint8_t values[6], I2C_HandleTypeDef* hi2c) {

}

uint8_t readBME280_id_reg(I2C_HandleTypeDef* hi2c, uint8_t bme) {
	uint8_t bme_addr = 0;
	if (bme == 0) {
		bme_addr = BME280_ADDR_0;
	} else if (bme == 1) {
		bme_addr = BME280_ADDR_1;
	}

	return i2c_read_reg(hi2c, bme_addr, &ID_REG);
}
