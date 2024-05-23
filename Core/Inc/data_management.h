/*
 * data_management.h
 *
 *  Created on: Mar 24, 2024
 *      Author: aidencontini
 */

#ifndef INC_DATA_MANAGEMENT_H_
#define INC_DATA_MANAGEMENT_H_
#include "stm32f4xx_hal.h"
#include "spi_driver.h"
#include <stdlib.h>

#define PAGE_SIZE 		256 			// 2048 bits
#define NUM_OF_PAGES 	65536
#define BLOCK_32K		256000
#define READ_SIZE		24
extern uint8_t FLASH_WREN;			// Write enable to begin writing data
extern uint8_t FLASH_WRDIS;			// Write disable to disable writing data
extern uint8_t FLASH_PGWR;			// Page write to write an entire page out
extern uint8_t FLASH_READSR1;		// Read status register value
extern uint8_t FLASH_READEN;		// Read enable to begin reading data
extern uint8_t FLASH_READ_MANU;		// Instruction code to read the manufacturer ID
extern uint8_t FLASH_32K_ERS;		// Erase all memory within a 32K-byte block
extern uint8_t FLASH_ERASE;			// Erase the entire chip
extern uint8_t RST_EN;				// Reset enable
extern uint8_t DEV_RST;				// Reset device

extern uint8_t data_buffer[PAGE_SIZE];

typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin_CS;
} GPIO_Config;

GPIO_Config getGPIOConfig(uint8_t flashNo);

#endif /* INC_DATA_MANAGEMENT_H_ */
