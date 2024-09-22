#include "data_interpretter.h"

// Function to read a record from the binary file
int read_record(FILE *fp, RawRecord *rec) {
    size_t bytesRead = fread(rec, sizeof(RawRecord), 1, fp);
    return bytesRead == 1;
}

int read_BME280_Calib(FILE *fp, BME280_CalibData *calib) {
    // Read the raw calibration data
    uint8_t rawData[SIZE_OF_CALIB];
    size_t bytesRead = fread(rawData, 1, sizeof(rawData), fp);

    if (bytesRead != sizeof(rawData)) {
        return 0; // Error: Could not read the entire calibration data
    }

    // Map the data to the struct fields
    bytesRead = read_BME280_calib(rawData, calib);

    return bytesRead == 1;
}

float convertToAccel(int16_t rawAccel) {
    rawAccel = rawAccel & 0x1FFF; // Mask the first 3 bits
    return (float) (twos_complement(rawAccel, ACCEL_BIT_SIZE)  * ACCEL_SENSITIVITY);
}

Record convertRaw(RawRecord *rec, BME280_CalibData *calib0, BME280_CalibData *calib1) {
    Record converted;
    converted.timestamp = rec->timestamp;
    converted.x_accel = convertToAccel(rec->x_accel);
    converted.y_accel = convertToAccel(rec->y_accel);
    converted.z_accel = convertToAccel(rec->z_accel);

    BME280_TempData tempData0 = compensate_T(rec->temperature0, calib0->dig_T1, calib0->dig_T2, calib0->dig_T3);
    converted.temperature0 = tempData0.temperature;
    converted.pressure0 = compensate_P(tempData0.t_fine, rec->pressure0, 
                calib0->dig_P1, calib0->dig_P2, calib0->dig_P3, calib0->dig_P4, calib0->dig_P5, 
                calib0->dig_P6, calib0->dig_P7, calib0->dig_P8, calib0->dig_P9);
    converted.humidity0 = compensate_H(tempData0.t_fine, rec->humidity0, calib0->dig_H1, calib0->dig_H2, 
                calib0->dig_H3, calib0->dig_H4, calib0->dig_H5, calib0->dig_H6);
    
    BME280_TempData tempData1 = compensate_T(rec->temperature1, calib1->dig_T1, calib1->dig_T2, calib1->dig_T3);
    converted.temperature1 = tempData1.temperature;
    converted.pressure1 = compensate_P(tempData1.t_fine, rec->pressure1, 
                calib1->dig_P1, calib1->dig_P2, calib1->dig_P3, calib1->dig_P4, calib1->dig_P5, 
                calib1->dig_P6, calib1->dig_P7, calib1->dig_P8, calib1->dig_P9);
    converted.humidity1 = compensate_H(tempData1.t_fine, rec->humidity1, calib1->dig_H1, calib1->dig_H2,
                calib1->dig_H3, calib1->dig_H4, calib1->dig_H5, calib1->dig_H6);
    
    converted.state = rec->status_byte >> 5;
    converted.status_code = hex_to_ascii_string(rec->status_byte & 0x1F);
    return converted;
}

int twos_complement(uint16_t value, int bits) {
    int val = 0;
    val = (int) value;
    if (val & (1 << (bits-1))){
        val -= 1 << bits;
    }
    return val;
}

uint16_t swap16(uint16_t val) {
    return (val << 8) | (val >> 8);
}

char* hex_to_ascii_string(unsigned int hex_value) {
    // Allocate memory for the output string (5 characters for "0xFF" + null terminator)
    char *output_str = (char*)malloc(6 * sizeof(char));
    
    if (output_str == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    // Convert the hexadecimal value to its ASCII string representation
    sprintf(output_str, "0x%02X", hex_value);

    return output_str;
}

// Function to write the record to CSV
void write_record_to_csv(FILE *fp, Record *rec) {
    // Write CSV row
    fprintf(fp,
            "%u,%f,%f,%f,%f,%f,%f,%f,%f,%f,%u,%s\n",
            rec->timestamp,
            rec->x_accel,
            rec->y_accel,
            rec->z_accel,
            rec->pressure0,
            rec->temperature0,
            rec->humidity0,
            rec->pressure1,
            rec->temperature1,
            rec->humidity1,
            rec->state,
            rec->status_code);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input binary file> <input BME280 calib 0> <input BME280 calib 1>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_filename = argv[1];
    const char *calib_filename0 = argv[2];
    const char *calib_filename1 = argv[3];

    FILE *calib_fp0 = fopen(calib_filename0, "rb");
    if (!calib_fp0) {
        perror("Failed to open calibration file 0");
        return EXIT_FAILURE;
    }

    FILE *calib_fp1 = fopen(calib_filename1, "rb");
    if (!calib_fp1) {
        perror("Failed to open calibration file 1");
        fclose(calib_fp0);
        return EXIT_FAILURE;
    }

    BME280_CalibData calib0;
    BME280_CalibData calib1;

    if (!read_BME280_Calib(calib_fp0, &calib0)) {
        fprintf(stderr, "Failed to read calibration data 0\n");
        fclose(calib_fp0);
        fclose(calib_fp1);
        return EXIT_FAILURE;
    }

    if (!read_BME280_Calib(calib_fp1, &calib1)) {
        fprintf(stderr, "Failed to read calibration data 1\n");
        fclose(calib_fp0);
        fclose(calib_fp1);
        return EXIT_FAILURE;
    }

    fclose(calib_fp0);
    fclose(calib_fp1);

    FILE *bin_fp = fopen(input_filename, "rb");
    if (!bin_fp) {
        perror("Failed to open binary file");
        return EXIT_FAILURE;
    }

    FILE *csv_fp = fopen("graphs/output.csv", "w");
    if (!csv_fp) {
        perror("Failed to open CSV file");
        fclose(bin_fp);
        return EXIT_FAILURE;
    }

    // Write CSV header
    fprintf(csv_fp, "Timestamp,X_Accel,Y_Accel,Z_Accel,Pressure0,Temperature0,Humidity0,Pressure1,Temperature1,Humidity1,State,StatusCode\n");

    RawRecord rec;
    size_t records_read = 0;

    while (read_record(bin_fp, &rec)) {
        Record converted = convertRaw(&rec, &calib0, &calib1);        
        write_record_to_csv(csv_fp, &converted);
        records_read++;

        // If we have read RECORDS_PER_PAGE, skip the PADDING_SIZE bytes
        if (records_read == RECORDS_PER_PAGE) {
            fseek(bin_fp, PADDING_SIZE, SEEK_CUR);
            records_read = 0; // Reset the record counter for the next page
        }
    }

    fclose(bin_fp);
    fclose(csv_fp);

    // Open the file in write mode (creates if doesn't exist, overwrites if exists)
    FILE *file = fopen("sensor_calibration/bme280_calib.csv", "w");
    if (file == NULL) {
        perror("Unable to open the file");
        return EXIT_FAILURE;
    }

    // Write the CSV header
    fprintf(file, "sensor,dig_T1,dig_T2,dig_T3,dig_P1,dig_P2,dig_P3,dig_P4,dig_P5,dig_P6,dig_P7,dig_P8,dig_P9,dig_H1,dig_H2,dig_H3,dig_H4,dig_H5,dig_H6\n");

    // Write the calibration data values to the file
    fprintf(file, "%u, %u,%d,%d,%u,%d,%d,%d,%d,%d,%d,%d,%d,%u,%d,%u,%d,%d,%d\n",
            0, calib0.dig_T1, calib0.dig_T2, calib0.dig_T3,
            calib0.dig_P1, calib0.dig_P2, calib0.dig_P3,
            calib0.dig_P4, calib0.dig_P5, calib0.dig_P6,
            calib0.dig_P7, calib0.dig_P8, calib0.dig_P9,
            calib0.dig_H1, calib0.dig_H2, calib0.dig_H3,
            calib0.dig_H4, calib0.dig_H5, calib0.dig_H6);

    fprintf(file, "%u, %u,%d,%d,%u,%d,%d,%d,%d,%d,%d,%d,%d,%u,%d,%u,%d,%d,%d\n",
            1, calib1.dig_T1, calib1.dig_T2, calib1.dig_T3,
            calib1.dig_P1, calib1.dig_P2, calib1.dig_P3,
            calib1.dig_P4, calib1.dig_P5, calib1.dig_P6,
            calib1.dig_P7, calib1.dig_P8, calib1.dig_P9,
            calib1.dig_H1, calib1.dig_H2, calib1.dig_H3,
            calib1.dig_H4, calib1.dig_H5, calib1.dig_H6);

    // Close the file
    fclose(file);

    return EXIT_SUCCESS;
}
