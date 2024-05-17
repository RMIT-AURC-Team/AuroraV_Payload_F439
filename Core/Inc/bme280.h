/*
 * bme280.h
 *
 *  Created on: May 11, 2024
 *      Author: aidencontini
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_

#include "stm32f4xx_hal.h"
#include "i2c_driver.h"

#define MASK_1kHz 		0x3F
#define MASK_100G 		0x00
#define CTRL_MEAS_MSK 	0x27
#define CTRL_HUM_MSK  	0x01
#define RST		  		0xB6
#define CONF_MSK  		0x00
#define CALIB_CNT_1		25
#define CALIB_CNT_2		7

extern const uint8_t BME280_ADDR_0;		// 7bit device address
extern const uint8_t BME280_ADDR_1;		// 7bit device address
extern uint8_t ID_REG;
extern uint8_t RST_REG;
extern uint8_t CTRL_HUM_REG;
extern uint8_t CTRL_MEAS_REG;
extern uint8_t CONF_REG;
extern uint8_t OUT_PRESS_L;
extern uint8_t OUT_PRESS_H;
extern uint8_t OUT_TEMP_L;
extern uint8_t OUT_TEMP_H;
extern uint8_t OUT_HUM_L;
extern uint8_t OUT_HUM_H;
extern uint8_t CALIB_ST_1;
extern uint8_t CALIB_ST_2;

typedef struct {
	uint8_t bme_addr;
} BME280_Config;

HAL_StatusTypeDef init_bme280(I2C_HandleTypeDef* hi2c, uint8_t bme);
void readTempHumPres(uint8_t values[6], I2C_HandleTypeDef* hi2c, uint8_t bme);
uint8_t readBME280_id_reg(I2C_HandleTypeDef* hi2c, uint8_t bme);
void readBME280_calib(I2C_HandleTypeDef* hi2c, uint8_t bme, uint8_t calibration1[CALIB_CNT_1], uint8_t calibration2[CALIB_CNT_2]);
void swap(uint8_t* a, uint8_t* b);
BME280_Config getBME280Config(uint8_t bme);

#endif /* INC_BME280_H_ */
