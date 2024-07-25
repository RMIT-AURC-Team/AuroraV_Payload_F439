/*
 * peripheral_driver.h
 *
 *  Created on: May 22, 2024
 *      Author: aidencontini
 */

#ifndef INC_PERIPHERAL_DRIVER_H_
#define INC_PERIPHERAL_DRIVER_H_

#include "main.h"
#include "uart_transmit.h"
#include "data_management.h"
#include "data_output_spi.h"
#include "accelerometer.h"
#include "can_driver.h"

/**
 * 0 = Online; 1 = Offline;
 *
 * Bits				Peripheral
 * 7				RESERVED (0)
 * 6				RESEVRED (0)
 * 5				RESERVED (0)
 * 4				BME280_1
 * 3				BME280_0
 * 2				Accelerometer
 * 1				SPIFlash_1
 * 0				SPIFlash_0
 */
uint8_t systemStatus(SPI_HandleTypeDef *hspi1, SPI_HandleTypeDef *hspi2, I2C_HandleTypeDef* hi2c1, I2C_HandleTypeDef* hi2c2);
void heartbeatUART(UART_HandleTypeDef *huart);
void eraseFlashSPI(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, GPIO_Config chip_select);
void readFlashToUART(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, GPIO_Config chip_select);
void readFlashManuSPI(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, GPIO_Config chip_select);
void writePageSPI_W(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, GPIO_Config chip_select);
void checkAccelWhoAmI(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef *huart);
void accelToUART(UART_HandleTypeDef *huart);
void readTempSensorID(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef *huart, uint8_t tempNo);
void readTempCalibration(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef *huart, uint8_t tempNo);
void readTempSensor(UART_HandleTypeDef *huart, uint8_t tempNo);
void readAllSensors(I2C_HandleTypeDef* hi2c_accel, I2C_HandleTypeDef* hi2c_temp, RTC_HandleTypeDef* hrtc);
void resetSPIFlash(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, GPIO_Config chip_select);


#endif /* INC_PERIPHERAL_DRIVER_H_ */
