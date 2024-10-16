/*
 * peripheral_driver.c
 *
 *  Created on: May 22, 2024
 *      Author: aidencontini
 */

#include "peripheral_driver.h"
/***************************************************************************************************************
 * Generic Functions
 */
uint8_t systemStatus(SPI_HandleTypeDef *hspi1, SPI_HandleTypeDef *hspi2, I2C_HandleTypeDef* hi2c1, I2C_HandleTypeDef* hi2c2) {
	uint8_t retVal = 0x00;

	// Check BME280_1
	if(readBME280_id_reg(hi2c1, 1) != 0x60) {			// Expected 0x60
		retVal  = retVal | 0x10;
	}

	// Check BME280_0
	if(readBME280_id_reg(hi2c1, 0) != 0x60) {			// Expected 0x60
		retVal  = retVal | 0x08;
	}

	// Check Accelerometer
	if(readAccel_whoami(hi2c2) != 0xE5) {				// Expected 0xE5
		retVal  = retVal | 0x04;
	}

	// Check SPIFlash_1
	uint8_t manu[2] = {0, 0};
	read_manufacturer_id(manu, hspi2, cs_spi2);
	if(manu[0] != 0xEF) {								// Expected 0xEF
		retVal  = retVal | 0x02;
	}

	// Check SPIFlash_0
	uint8_t manu0[2] = {0, 0};
	read_manufacturer_id(manu0, hspi1, cs_spi1);
	if(manu0[0] != 0xEF) {								// Expected 0xEF
		retVal  = retVal | 0x01;
	}
	return retVal;
}

void heartbeatUART(UART_HandleTypeDef *huart) {
	send_uart_hex(huart, 0x51);			// Transmit the heartbeat as device ID (0x51 = Q)
}

/***************************************************************************************************************
 * SPI Flash Functions
 */
void eraseFlashSPI(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, GPIO_Config chip_select) {
	if (erase_chip_spi(hspi, chip_select) == HAL_OK) {
		send_uart_string(huart, "Successful Chip Erase\r\n");
	} else {
		send_uart_string(huart, "Error during chip erase. Please check the connection and try again.\r\n");
	}
}

void readFlashToUART(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, GPIO_Config chip_select) {
	uint32_t num_of_pages = next_blank_page;
	if(num_of_pages == 0) {
		num_of_pages = PAGE_SIZE;
	}
	num_of_pages = num_of_pages/PAGE_SIZE;
	uint32_t address = 0;

	for (int i = 0; i < (num_of_pages); i++) {
		uint8_t page[PAGE_SIZE];
		read_page_spi(page, hspi, address, chip_select);
		uart_transmit_page(huart, page);						// Transmit the data//
		address += PAGE_SIZE;
	}
}

void readFlashManuSPI(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, GPIO_Config config) {
	uint8_t manu[2] = {0, 0};
	read_manufacturer_id(manu, hspi, config);
	send_uart_hex(huart, manu[0]);
	send_uart_hex(huart, manu[1]);
}

void writePageSPI_W(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, GPIO_Config config) {
	uint8_t data_out[PAGE_SIZE];
	for (int i = 0; i < PAGE_SIZE; i++) {
		data_out[i] = 0x77;		// Make all data in the page 'w'
	}
	write_data_spi_dma(data_out, hspi, next_blank_page, config);
	next_blank_page += PAGE_SIZE;

	send_uart_string(huart, "Successful Page Written\r\n");
}

void resetSPIFlash(SPI_HandleTypeDef *hspi, UART_HandleTypeDef *huart, GPIO_Config config) {
	software_reset(hspi, config);
	send_uart_string(huart, "Flash Chip Reset\r\n");
}
/***************************************************************************************************************
 * I2C Accelerometer Functions
 */
void checkAccelWhoAmI(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef *huart) {
	uint8_t whoami = readAccel_whoami(hi2c);
	send_uart_hex(huart, whoami);
}

void accelToUART(UART_HandleTypeDef *huart) {
	for (int i = 0; i < 6; i++) {
		send_uart_hex(huart, accel_data[i]);
	}
}

/***************************************************************************************************************
 * I2C BME280 Sensor Functions
 */
void readTempSensorID(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef *huart, uint8_t tempNo) {
	send_uart_hex(huart, readBME280_id_reg(hi2c, tempNo));
}

void readTempCalibration(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef *huart, uint8_t tempNo) {
	uint8_t calibration1[25];
	uint8_t calibration2[7];

	for (uint8_t i = 0; i < CALIB_CNT_1; i++) {
		calibration1[i] = 0x00;
		if (i < CALIB_CNT_2) {
			calibration2[i] = 0x00;
		}
	}

	readBME280_calib(hi2c, tempNo, calibration1, calibration2);

	for (int i = 0; i < 25; i++) {
	  send_uart_hex(huart, calibration1[i]);
	}

	for (int i = 0; i < 7; i++) {
	  send_uart_hex(huart, calibration2[i]);
	}
}

void readTempSensor(UART_HandleTypeDef *huart, uint8_t tempNo) {
	if (tempNo == 0) {
		for (int i = 0; i < 6; i++) {
			send_uart_hex(huart, bme280_data_0[i]);
		}
	} else if (tempNo == 1) {
		for (int i = 0; i < 6; i++) {
			send_uart_hex(huart, bme280_data_1[i]);
		}
	}
}

/***************************************************************************************************************
 * Process All Sensors
 */
void readAllSensors(I2C_HandleTypeDef* hi2c_accel, I2C_HandleTypeDef* hi2c_temp, RTC_HandleTypeDef* hrtc) {
	// Read Accelerometer Data
	readAccelerometer(accel_data, hi2c_accel);

	// Read BME280 Data
	readTempHumPres(bme280_data_0, hi2c_temp, 0);
	readTempHumPres(bme280_data_1, hi2c_temp, 1);

	uint16_t time = getTimestampMilliseconds(hrtc);
	uint8_t array_ptr = 0;

	// Store the new read in the buffer if there is space
	if (byte_tracker < (PAGE_SIZE - READ_SIZE)) {
		// Store the timestamp in the buffer (Little Endian)
		data_buffer_tx[buffer_tracker][byte_tracker + array_ptr] = (uint8_t) (time & 0xFF); 		// Least significant byte (LSB)
		array_ptr += 1;
		data_buffer_tx[buffer_tracker][byte_tracker + array_ptr] = (uint8_t) ((time >> 8) & 0xFF); // Most significant byte (MSB)
		array_ptr += 1;

		// Store the accelerometer data
		for (int i = 0; i < 6; i++) {
		  data_buffer_tx[buffer_tracker][byte_tracker + array_ptr] = accel_data[i];
		  array_ptr += 1;
		}

		// Store the BME280_0 Data
		for (int i = 0; i < 6; i++) {
		  data_buffer_tx[buffer_tracker][byte_tracker + array_ptr] = bme280_data_0[i];
		  array_ptr += 1;
		}

		// Store the BME280_1 Data
		for (int i = 0; i < 6; i++) {
		  data_buffer_tx[buffer_tracker][byte_tracker + array_ptr] = bme280_data_1[i];
		  array_ptr += 1;
		}

		// Store the flight state and peripheral status in the final byte
		data_buffer_tx[buffer_tracker][byte_tracker + array_ptr] = combine_system_status();
		array_ptr += 1;

		byte_tracker = byte_tracker + (array_ptr);
	}
}


