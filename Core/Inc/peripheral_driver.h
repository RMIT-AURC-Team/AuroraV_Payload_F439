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

void heartbeatUART(UART_HandleTypeDef *huart);
void eraseFlashSPI(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, uint8_t flashNo);
void readFlashToUART(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, uint8_t flashNo);
void readFlashManuSPI(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, uint8_t flashNo);
void writePageSPI_W(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, uint8_t flashNo);
void checkAccelWhoAmI(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef *huart);
void accelToUART(UART_HandleTypeDef *huart);
void readTempSensorID(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef *huart, uint8_t tempNo);
void readTempCalibration(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef *huart, uint8_t tempNo);
void readTempSensor(UART_HandleTypeDef *huart, uint8_t tempNo);
void readAllSensors(I2C_HandleTypeDef* hi2c_accel, I2C_HandleTypeDef* hi2c_temp, RTC_HandleTypeDef* hrtc);
void resetSPIFlash(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, uint8_t flashNo);


#endif /* INC_PERIPHERAL_DRIVER_H_ */
