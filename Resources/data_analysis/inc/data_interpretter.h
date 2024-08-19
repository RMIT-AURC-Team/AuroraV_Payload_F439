#ifndef DATA_INTERPRETTER_H
#define DATA_INTERPRETTER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "BME280_comp.h"

#define ACCEL_SENSITIVITY   0.049
#define ACCEL_BIT_SIZE      13
#define PAGE_SIZE 256
#define RECORD_SIZE 21
#define PADDING_SIZE 4
#define RECORDS_PER_PAGE (PAGE_SIZE / RECORD_SIZE)

enum FlightState {
    GROUNDED = 0,
    LOADED = 1,
    ASCENT = 2,
    DESCENT = 3,
    RECOVERY = 4
};

// Define a structure to represent a record
#pragma pack(push, 1) // Ensure no padding
typedef struct {
    uint16_t timestamp;
    int16_t x_accel;
    int16_t y_accel;
    int16_t z_accel;
    uint16_t pressure0;
    int16_t temperature0;
    uint16_t humidity0;
    uint16_t pressure1;
    int16_t temperature1;
    uint16_t humidity1;
    uint8_t status_byte; // Byte 20
} RawRecord;
#pragma pack(pop)

typedef struct {
    int timestamp;
    float x_accel;
    float y_accel;
    float z_accel;
    double pressure0;
    double temperature0;
    double humidity0;
    double pressure1;
    double temperature1;
    double humidity1;
    enum FlightState state;
    char* status_code;
} Record;

int read_record(FILE *fp, RawRecord *rec);
float convertToAccel(int16_t rawAccel);
Record convertRaw(RawRecord *rec, BME280_CalibData *calib0, BME280_CalibData *calib1);

/**
 * @brief Converts an unsigned integer to its two's complement signed integer representation.
 *
 * This function takes an unsigned integer value and the number of bits in its original
 * representation, and converts it to its corresponding signed integer value using two's
 * complement notation.
 *
 * @param value The unsigned integer to be converted.
 * @param bits The number of bits in the original representation.
 * @return The signed integer representation of the input value.
 */
int twos_complement(uint16_t value, int bits);

// Function to swap the bytes of a 16-bit value
uint16_t swap16(uint16_t val);
char* hex_to_ascii_string(unsigned int hex_value);
void write_record_to_csv(FILE *fp, Record *rec);

#endif  // DATA_INTERPRETTER_H