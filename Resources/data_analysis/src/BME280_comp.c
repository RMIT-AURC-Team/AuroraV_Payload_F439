#include "BME280_comp.h"

int read_BME280_calib(uint8_t* rawData, BME280_CalibData * calib) {
  // Map the data to the struct fields
  calib->dig_T1 = (uint16_t)(rawData[0] | (rawData[1] << 8));
  calib->dig_T2 = (int16_t)(rawData[2] | (rawData[3] << 8));
  calib->dig_T3 = (int16_t)(rawData[4] | (rawData[5] << 8));
  calib->dig_P1 = (uint16_t)(rawData[6] | (rawData[7] << 8));
  calib->dig_P2 = (int16_t)(rawData[8] | (rawData[9] << 8));
  calib->dig_P3 = (int16_t)(rawData[10] | (rawData[11] << 8));
  calib->dig_P4 = (int16_t)(rawData[12] | (rawData[13] << 8));
  calib->dig_P5 = (int16_t)(rawData[14] | (rawData[15] << 8));
  calib->dig_P6 = (int16_t)(rawData[16] | (rawData[17] << 8));
  calib->dig_P7 = (int16_t)(rawData[18] | (rawData[19] << 8));
  calib->dig_P8 = (int16_t)(rawData[20] | (rawData[21] << 8));
  calib->dig_P9 = (int16_t)(rawData[22] | (rawData[23] << 8));
  calib->dig_H1 = rawData[24];

  // Map the humidity calibration data
  calib->dig_H2 = (int16_t)(rawData[25] | (rawData[26] << 8));
  calib->dig_H3 = rawData[27];

  // dig_H4 spans two bytes and uses bits 3:0 from 0xE4 and bits 7:4 from 0xE5
  calib->dig_H4 = (int16_t)((rawData[28] << 4) | (rawData[29] & 0x0F));

  // dig_H5 spans two bytes and uses bits 3:0 from 0xE5 and bits 7:4 from 0xE6
  calib->dig_H5 = (int16_t)((rawData[30] << 4) | ((rawData[29] >> 4) & 0x0F));

  // dig_H6 is a single byte
  calib->dig_H6 = (int8_t)rawData[31];

  return 1; // Success
}

BME280_TempData compensate_T(int32_t adc_T, uint16_t dig_T1, int16_t dig_T2, int16_t dig_T3) {
  BME280_TempData tempData;
  
  double var1, var2;
  adc_T = adc_T << 4;
  var1 = (((double)adc_T)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
  var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0) * (((double)adc_T)/131072.0 - ((double) dig_T1)/8192.0)) * ((double)dig_T3);
  tempData.t_fine = (int32_t)(var1 + var2);
  tempData.temperature = (var1 + var2) / 5120.0;
  return tempData;
}

double compensate_P(
  int32_t t_fine, int32_t adc_P, uint16_t dig_P1, int16_t dig_P2, int16_t dig_P3, int16_t dig_P4, int16_t dig_P5, int16_t dig_P6, 
  int16_t dig_P7, int16_t dig_P8, int16_t dig_P9
) {
  double var1, var2, p;
  adc_P = adc_P << 4;
  var1 = ((double)t_fine/2.0) - 64000.0;
  var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
  var2 = var2 + var1 * ((double)dig_P5) * 2.0;
  var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);
  var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
  var1 = (1.0 + var1 / 32768.0)*((double)dig_P1);
  if (var1 == 0.0) {
    return 0; // avoid exception caused by division by zero
  }
  p = 1048576.0 - (double)adc_P;
  p = (p - (var2 / 4096.0)) * 6250.0 / var1;
  var1 = ((double)dig_P9) * p * p / 2147483648.0;
  var2 = p * ((double)dig_P8) / 32768.0;
  p = p + (var1 + var2 + ((double)dig_P7)) / 16.0;
  return p;
}

double compensate_H (int32_t t_fine, uint32_t adc_H, uint8_t dig_H1, int16_t dig_H2, uint8_t dig_H3, int16_t dig_H4, int16_t dig_H5, int8_t dig_H6) {
  double var_H;
  var_H = (((double)t_fine) - 76800.0);
  var_H = (adc_H - (((double)dig_H4) * 64.0 + ((double)dig_H5) / 16384.0 *
  var_H)) * (((double)dig_H2) / 65536.0 * (1.0 + ((double)dig_H6) /
  67108864.0 * var_H *
  (1.0 + ((double)dig_H3) / 67108864.0 * var_H)));
  var_H = var_H * (1.0 - ((double)dig_H1) * var_H / 524288.0);
  if (var_H > 100.0)
    var_H = 100.0;
  else if (var_H < 0.0)
    var_H = 0.0;
  return var_H;
}
