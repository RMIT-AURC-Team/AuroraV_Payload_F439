/*
 * rtc.c
 *
 *  Created on: Apr 4, 2024
 *      Author: aidencontini
 */

#include "rtc.h"

RTC_TimeTypeDef defaultTime = {
    .Hours = 0, // Set default hours (e.g., noon)
    .Minutes = 0, // Set default minutes
    .Seconds = 0, // Set default seconds
    .SubSeconds = 0, // Set default subseconds (if applicable)
    .DayLightSaving = RTC_DAYLIGHTSAVING_NONE, // Adjust as needed
    .StoreOperation = RTC_STOREOPERATION_RESET // Adjust as needed
};

RTC_DateTypeDef defaultDate = {
    .WeekDay = RTC_WEEKDAY_MONDAY, // Set default weekday (e.g., Monday)
    .Month = RTC_MONTH_JANUARY, // Set default month (e.g., January)
    .Date = 1, // Set default day of the month
    .Year = 24 // Set default year (e.g., 2021)
};

void initialise_rtc_default(RTC_HandleTypeDef* hrtc) {
    // Enable access to the backup domain
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    // Reset the RTC
    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();

    // Initialize RTC hardware
    HAL_RTC_MspInit(hrtc);

    // Set default time and date
    HAL_RTC_SetTime(hrtc, &defaultTime, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(hrtc, &defaultDate, RTC_FORMAT_BIN);

    // Disable access to the backup domain
    HAL_PWR_DisableBkUpAccess();
}

uint32_t getTimestampMilliseconds(RTC_HandleTypeDef* hrtc) {
    RTC_TimeTypeDef rtcTime;
    RTC_DateTypeDef rtcDate;

    // Read RTC time and date
    HAL_RTC_GetTime(hrtc, &rtcTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(hrtc, &rtcDate, RTC_FORMAT_BIN);

    // Calculate timestamp in milliseconds
    uint32_t timestampSeconds = rtcTime.Seconds + rtcTime.Minutes * 60 + rtcTime.Hours * 3600;
    uint32_t timestampMilliseconds = timestampSeconds * 1000;
    float subsecondsFraction = (float)(rtcTime.SecondFraction - rtcTime.SubSeconds) / (rtcTime.SecondFraction + 1);
    timestampMilliseconds += subsecondsFraction * 1000;

    return timestampMilliseconds;
}


