#ifndef BME280_COMP_H
#define BME280_COMP_H

#include <stdint.h>
#include <stdio.h>

#define SIZE_OF_CALIB 31

typedef struct {
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;
} BME280_CalibData;

typedef struct {
    int32_t t_fine;
    double temperature;
} BME280_TempData;

int read_BME280_calib(uint8_t* rawData, BME280_CalibData * calib);

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
BME280_TempData compensate_T(uint32_t adc_T, uint16_t dig_T1, int16_t dig_T2, int16_t dig_T3);

// Returns pressure in Pa as double. Output value of “96386.2” equals 96386.2 Pa = 963.862 hPa
double compensate_P(
  int32_t t_fine, uint32_t adc_P,
  uint16_t dig_P1, int16_t dig_P2, int16_t dig_P3, 
  int16_t dig_P4, int16_t dig_P5, int16_t dig_P6, 
  int16_t dig_P7, int16_t dig_P8, int16_t dig_P9
);

// Returns humidity in %rH as as double. Output value of “46.332” represents 46.332 %rH
double compensate_H (
  int32_t t_fine, uint32_t adc_H, 
  uint8_t dig_H1, int16_t dig_H2, uint8_t dig_H3, 
  int16_t dig_H4, int16_t dig_H5, int8_t dig_H6
);

#endif  // BME280_COMP_H
