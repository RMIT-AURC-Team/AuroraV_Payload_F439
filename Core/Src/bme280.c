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
uint8_t CALIB_ST_1			= 0x88;
uint8_t CALIB_ST_2			= 0xE1;


HAL_StatusTypeDef init_bme280(I2C_HandleTypeDef* hi2c, uint8_t bme) {
	uint8_t bme_addr = getBME280Config(bme).bme_addr;
	HAL_StatusTypeDef ret;
	uint8_t control_reg = 0x00;

	// Reset the sensor
	ret =  i2c_write_reg(hi2c, bme_addr, &RST_REG, RST);

	// Set humidity control register
	i2c_read_reg(hi2c, &control_reg, bme_addr, &CTRL_HUM_REG);
	control_reg = control_reg & 0xF8;
	control_reg = control_reg | CTRL_HUM_MSK;
	ret = i2c_write_reg(hi2c, bme_addr, &CTRL_HUM_REG, control_reg);

	// Set measure control register
	ret = i2c_write_reg(hi2c, bme_addr, &CTRL_MEAS_REG, CTRL_MEAS_MSK);

	// Set the config register
	ret = i2c_write_reg(hi2c, bme_addr, &CONF_REG, CONF_MSK);

	return ret;
}

void readTempHumPres(uint8_t values[6], I2C_HandleTypeDef* hi2c, uint8_t bme) {
	uint8_t readings[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t bme_addr = getBME280Config(bme).bme_addr;
	i2c_burst_read(hi2c, bme_addr, OUT_PRESS_H, 8, readings);

	// Clear the 0 values on the xlsb registers
	uint8_t j = 0;
    for(uint8_t i = 0; i < 8; i++) {
        if(i != 2 && i != 5) {  // if the index is not 2 or 5
        	values[j] = readings[i];
            j++;
        }
    }

    // Swap elements to make the array little endian
    swap(&values[0], &values[1]);
    swap(&values[2], &values[3]);
    swap(&values[4], &values[5]);
}

uint8_t readBME280_id_reg(I2C_HandleTypeDef* hi2c, uint8_t bme) {
	uint8_t bme_addr = getBME280Config(bme).bme_addr;
	uint8_t data = 0x00;
	i2c_read_reg(hi2c, &data, bme_addr, &ID_REG);

	return data;
}

void readBME280_calib(I2C_HandleTypeDef* hi2c, uint8_t bme, uint8_t calibration1[CALIB_CNT_1], uint8_t calibration2[CALIB_CNT_2]) {
	uint8_t bme_addr = getBME280Config(bme).bme_addr;
	i2c_burst_read(hi2c, bme_addr, CALIB_ST_1, CALIB_CNT_1, calibration1);
	i2c_burst_read(hi2c, bme_addr, CALIB_ST_2, CALIB_CNT_2, calibration2);
}

BME280_Config getBME280Config(uint8_t bme) {
	BME280_Config config;
	if (bme == 0) {
		config.bme_addr = BME280_ADDR_0;
	} else if (bme == 1) {
		config.bme_addr = BME280_ADDR_1;
	}
    return config;
}

void swap(uint8_t* a, uint8_t* b) {
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

