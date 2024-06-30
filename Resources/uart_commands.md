# Possible UART Commands

| Command ID | Suffix | Function | Expected Output |
| -------- | ------- | ------- | ------- |
| General Functions| | | |
| h | X | Send heartbeat over UART | 0x5100 |
| SPI Flash Functions | | | |
| e | 0 or 1 | Erase SPI flash as per index | Successful Chip Erase |
| r | 0 or 1 | Read full contents of SPI flash as per index | Full page output in Binary |
| m | 0 or 1 | Read manufacturer ID of SPI flash as per index | 0xEF17 |
| w | 0 or 1 | Write a full page of ASCII 'w' to SPI flash as per index | None |
| Acclerometer I2C Functions | | | |
| c | X | Read accelerometer WhoAmI register | 0xE5 |
| a | X | Read last recorded accelerometer x/y/z data | 6 byte hexademical output (LE) |
| BME280 Functions | | | |
| b | 0 or 1 | Read temperature sensor ID as per index | 0x60 |
| p | 0 or 1 | Read temperature sensor calibration registers as per index | 32 byte hexademical output |
| t | 0 or 1 | Read last recorded temperature data press/temp/hum data | 8 byte hexademical output (LE) |