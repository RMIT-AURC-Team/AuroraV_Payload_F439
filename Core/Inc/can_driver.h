/*
 * can_driver.h
 *
 *  Created on: Jul 24, 2024
 *      Author: aidencontini
 */

#ifndef INC_CAN_DRIVER_H_
#define INC_CAN_DRIVER_H_

#include "stm32f4xx_hal.h"

#define CAN_PL_LGTH			8		// Size of the CAN payload in Bytes
#define CLK_SYNC_ID			0x7FF
#define TX_BME280_0			0x501
#define TX_BME280_1			0x502
#define TX_ACCEL			0x503
#define TX_STATUS			0x504
#define DUMMY_ID			0x700

void sendCAN_TxMessage(CAN_HandleTypeDef *hcan, uint8_t dataSize, uint8_t TxData[dataSize], uint32_t *CAN_TxMailbox, uint16_t payload_id);
CAN_TxHeaderTypeDef configureTxHeader(uint16_t payload_id, uint8_t dataSize);

#endif /* INC_CAN_DRIVER_H_ */
