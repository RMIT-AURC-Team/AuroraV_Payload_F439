/*
 * can_driver.c
 *
 *  Created on: Jul 24, 2024
 *      Author: aidencontini
 */

#include "can_driver.h"


void sendCAN_TxMessage(CAN_HandleTypeDef *hcan, uint8_t dataSize, uint8_t TxData[dataSize], uint32_t *CAN_TxMailbox, uint16_t payload_id) {
	CAN_TxHeaderTypeDef TxHeader = configureTxHeader(payload_id, dataSize);
	HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, CAN_TxMailbox);
}

CAN_TxHeaderTypeDef configureTxHeader(uint16_t payload_id, uint8_t dataSize) {
	CAN_TxHeaderTypeDef TxHeader;
	TxHeader.DLC = dataSize;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.StdId = payload_id;
	TxHeader.TransmitGlobalTime = DISABLE;
	return TxHeader;
}


