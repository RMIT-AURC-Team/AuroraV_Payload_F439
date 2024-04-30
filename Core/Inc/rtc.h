/*
 * rtc.h
 *
 *  Created on: Apr 4, 2024
 *      Author: aidencontini
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "stm32f4xx_hal.h"

void initialise_rtc_default(RTC_HandleTypeDef* hrtc);
uint32_t getTimestampMilliseconds(RTC_HandleTypeDef* hrtc);

#endif /* INC_RTC_H_ */
