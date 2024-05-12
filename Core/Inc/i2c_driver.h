/*
 * i2c_driver.h
 *
 *  Created on: May 12, 2024
 *      Author: aidencontini
 */

#ifndef INC_I2C_DRIVER_H_
#define INC_I2C_DRIVER_H_

#include "stm32f4xx_hal.h"

uint8_t i2c_read_reg(I2C_HandleTypeDef* hi2c, uint8_t dev_addr, uint8_t* register_addr);
HAL_StatusTypeDef i2c_write_reg(I2C_HandleTypeDef* hi2c, uint8_t dev_addr, uint8_t* register_addr, uint8_t data);

#endif /* INC_I2C_DRIVER_H_ */
