/*
 * accelerometer.h
 *
 *  Created on: Apr 1, 2024
 *      Author: aidencontini
 */

#ifndef INC_ACCELEROMETER_H_
#define INC_ACCELEROMETER_H_

#include "stm32f4xx_hal.h"

#define MASK_1kHz 0x3F
#define MASK_100G 0x00

extern const uint8_t H3LIS331HH_ADDR;		// 7bit device address
extern uint8_t CTRL_REG1;
extern uint8_t CTRL_REG4;
extern uint8_t OUT_X_L;
extern uint8_t OUT_X_H;
extern uint8_t OUT_Y_L;
extern uint8_t OUT_Y_H;
extern uint8_t OUT_Z_L;
extern uint8_t OUT_Z_H;

HAL_StatusTypeDef readAccelerometer(uint8_t values[6], I2C_HandleTypeDef* hi2c);
HAL_StatusTypeDef init_accel(I2C_HandleTypeDef* hi2c);
HAL_StatusTypeDef readAccel_ctrl_rg1(I2C_HandleTypeDef* hi2c);

#endif /* INC_ACCELEROMETER_H_ */
