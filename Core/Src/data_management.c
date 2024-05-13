/*
 * data_management.c
 *
 *  Created on: Apr 6, 2024
 *      Author: aidencontini
 */

#include "data_management.h"

uint8_t FLASH_WREN 		= 0x06;			// Write enable to begin writing data
uint8_t FLASH_WRDIS 	= 0x04;			// Write disable to disable writing data
uint8_t FLASH_PGWR 		= 0x02;			// Page write to write an entire page out
uint8_t FLASH_READSR1 	= 0x05;			// Read status register value
uint8_t FLASH_READEN 	= 0x03;			// Read enable to begin reading data
uint8_t FLASH_READ_MANU	= 0x90;			// Instruction code to read the manufacturer ID
uint8_t FLASH_32K_ERS	= 0x52;			// Erase all memory within a 32K-byte block
uint8_t FLASH_ERASE		= 0x60;			// Erase the entire chip

GPIO_Config getGPIOConfig(uint8_t flashNo) {
    GPIO_Config config;

    if (flashNo == 0) {
        config.GPIOx = GPIOD;
        config.GPIO_Pin_CS = GPIO_PIN_2;
    } else if (flashNo == 1) {
        config.GPIOx = GPIOB;
        config.GPIO_Pin_CS = GPIO_PIN_12;
    }

    return config;
}
