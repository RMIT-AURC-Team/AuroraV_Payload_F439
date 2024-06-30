/*
 * accelerometer.h
 *
 *  Created on: Apr 1, 2024
 *      Author: aidencontini
 */

#ifndef INC_ACCELEROMETER_H_
#define INC_ACCELEROMETER_H_

#include "stm32f4xx_hal.h"
#include "i2c_driver.h"

#define MASK_50Hz 	0x09
#define MASK_1kHz 	0x0A
#define RIGHT_JUST	0x0B
#define START_MEAS	0x08

HAL_StatusTypeDef init_accel(I2C_HandleTypeDef* hi2c);
uint8_t readAccel_whoami(I2C_HandleTypeDef* hi2c);
void readAccelerometer(uint8_t values[6], I2C_HandleTypeDef* hi2c);
HAL_StatusTypeDef writeOffset(I2C_HandleTypeDef* hi2c);

#endif /* INC_ACCELEROMETER_H_ */
