/*
 * i2c_driver.h
 *
 *  Created on: May 12, 2024
 *      Author: aidencontini
 */

#ifndef INC_I2C_DRIVER_H_
#define INC_I2C_DRIVER_H_

#include "stm32f4xx_hal.h"

/**
 * @brief Reads a register of a device via I2C.
 *
 * This function reads a register of a device using I2C communication. It first transmits the register address to the device,
 * then receives the data from the register.
 *
 * @param hi2c Handle to the I2C peripheral to use for communication.
 * @param dev_addr Address of the device on the I2C bus.
 * @param register_addr Pointer to the register address to be read.
 *
 * @return The data read from the register.
 */
uint8_t i2c_read_reg(I2C_HandleTypeDef* hi2c, uint8_t dev_addr, uint8_t* register_addr);

void i2c_burst_read(I2C_HandleTypeDef* hi2c, uint8_t dev_addr, uint8_t register_addr, uint8_t size, uint8_t data_buffer[size]);

/**
 * @brief Writes data to a register of a device via I2C.
 *
 * This function writes a byte of data to a register of a device using I2C communication. It first prepares the data to be transmitted,
 * which includes the register address and the data, then transmits the data to the device.
 *
 * @param hi2c Handle to the I2C peripheral to use for communication.
 * @param dev_addr Address of the device on the I2C bus.
 * @param register_addr Pointer to the register address to be written to.
 * @param data The data to be written to the register.
 *
 * @return HAL status of the I2C write operation. HAL_OK if the operation is successful; an error code otherwise.
 */
HAL_StatusTypeDef i2c_write_reg(I2C_HandleTypeDef* hi2c, uint8_t dev_addr, uint8_t* register_addr, uint8_t data);

#endif /* INC_I2C_DRIVER_H_ */
