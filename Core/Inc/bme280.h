/*
 * bme280.h
 *
 *  Created on: May 11, 2024
 *      Author: aidencontini
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_

#include "stm32f4xx_hal.h"

#define MASK_1kHz 0x3F
#define MASK_100G 0x00
#define CTRL_MEAS 0x27
#define CTRL_HUM  0x01
#define RST		  0xB6
#define CONF_MSK  0x00

extern const uint8_t BME280_ADDR;		// 7bit device address
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

HAL_StatusTypeDef init_bme280(I2C_HandleTypeDef* hi2c);
HAL_StatusTypeDef readTempHumPres(uint8_t values[6], I2C_HandleTypeDef* hi2c);
HAL_StatusTypeDef readBME280_id_reg(I2C_HandleTypeDef* hi2c);

#endif /* INC_BME280_H_ */
