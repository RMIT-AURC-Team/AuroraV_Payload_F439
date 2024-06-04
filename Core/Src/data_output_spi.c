/*
 * data_output.c
 *
 *  Created on: Mar 20, 2024
 *      Author: aidencontini
 */

#include "data_output_spi.h"

HAL_StatusTypeDef erase_chip_spi(SPI_HandleTypeDef *hspi, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);

	check_busy(hspi, flashNo, BSY_TIMEOUT_MS);

	write_enable_spi(hspi, flashNo);
	perform_operation(&FLASH_ERASE, hspi, config.GPIOx, config.GPIO_Pin_CS);

	check_busy(hspi, flashNo, CHIP_ERASE_TO);

	return HAL_OK;
}

HAL_StatusTypeDef erase_32k_spi(SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);

	check_busy(hspi, flashNo, BSY_TIMEOUT_MS);

	write_enable_spi(hspi, flashNo);
	perform_operation(&FLASH_32K_ERS, hspi, config.GPIOx, config.GPIO_Pin_CS);

	check_busy(hspi, flashNo, BLK_ERS_32K_TO);

	return HAL_OK;
}

void write_enable_spi(SPI_HandleTypeDef *hspi, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);
	perform_operation(&FLASH_WREN, hspi, config.GPIOx, config.GPIO_Pin_CS);
}

void write_disable_spi(SPI_HandleTypeDef *hspi, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);
	perform_operation(&FLASH_WRDIS, hspi, config.GPIOx, config.GPIO_Pin_CS);
}

void software_reset(SPI_HandleTypeDef *hspi, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);

	check_busy(hspi, flashNo, BSY_TIMEOUT_MS);

	perform_operation(&RST_EN, hspi, config.GPIOx, config.GPIO_Pin_CS);
	perform_operation(&DEV_RST, hspi, config.GPIOx, config.GPIO_Pin_CS);

	HAL_Delay(5);
}

uint8_t write_data_spi(uint8_t page[PAGE_SIZE], SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t flashNo) {
	GPIO_Config config = getGPIOConfig(flashNo);

	check_busy(hspi, flashNo, BSY_TIMEOUT_MS);

	// Send the write enable signal
	write_enable_spi(hspi, flashNo);
	spi_write_data(&FLASH_PGWR, PAGE_SIZE, page, hspi, addr, config.GPIOx, config.GPIO_Pin_CS);

	return 0;
}
