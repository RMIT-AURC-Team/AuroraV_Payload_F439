#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

uint16_t little_to_big(uint16_t value);
int twos_complement(uint16_t value, int bits);

void interpret_data(char* file_path, int skip_bytes) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Could not open file %s", file_path);
        return;
    }

    FILE *csv_file = fopen("log/interpretted_data.csv", "w");
    if (csv_file == NULL) {
        printf("Could not open log/interpretted_data.csv for writing");
        return;
    }

    // Write the header
    fprintf(csv_file, "Timestamp,X_Accel,Y_Accel,Z_Accel,Pressure1,Temperature1,Humidity1,Pressure2,Temperature2,Humidity2\n");

    fseek(file, skip_bytes, SEEK_SET);

    int byte_counter = 0;

    while (1) {
        if (byte_counter == 240) {
            fseek(file, 16, SEEK_CUR);
            byte_counter = 0;
        }

        uint16_t timestamp;
        int16_t int1, int2, int3, int4, int5, int6, int7, int8, int9;

        // Save the timestamp to file
        size_t result = fread(&timestamp, sizeof(uint16_t), 1, file);
        timestamp = ntohs(timestamp);  // Convert from network byte order to host byte order

        // Accel X
        result += fread(&int1, sizeof(int16_t), 1, file);
        int1 = int1 & 0x1FFF;  // Mask the first 3 bits
        // int1 = int1 >> 4;


        // Accel Y
        result += fread(&int2, sizeof(int16_t), 1, file);
        int2 = int2 & 0x1FFF;  // Mask the first 3 bits
        // int2 = int2 >> 4;


        // Accel Z
        result += fread(&int3, sizeof(int16_t), 1, file);
        int3 = int3 & 0x1FFF;  // Mask the first 3 bits
        // int3 = int3 >> 4;

        // Pressure 1
        result += fread(&int4, sizeof(int16_t), 1, file);
        int4 = le16toh(int4);

        // Temperature 1
        result += fread(&int5, sizeof(int16_t), 1, file);
        int5 = le16toh(int5);

        // Humidity 1
        result += fread(&int6, sizeof(int16_t), 1, file);
        int6 = le16toh(int6);

        // Pressure 2
        result += fread(&int7, sizeof(int16_t), 1, file);
        int7 = le16toh(int7);

        // Temperature 2
        result += fread(&int8, sizeof(int16_t), 1, file);
        int8 = le16toh(int8);

        // Humidity 2
        result += fread(&int9, sizeof(int16_t), 1, file);
        int9 = le16toh(int9);

        if (result < 10) {
            break;
        }

        // Convert int1, int2, and int3 to Gs
        float int1_g = (float) (twos_complement(int1, 13) * 0.049);
        float int2_g = (float) (twos_complement(int2, 13) * 0.049);
        float int3_g = (float) (twos_complement(int3, 13) * 0.049);

        // Write the data to the CSV file
        fprintf(csv_file, "%d,%f,%f,%f,%d,%d,%d,%d,%d,%d\n", timestamp, int1_g, int2_g, int3_g, int4, int5, int6, int7, int8, int9);

        byte_counter += 20;  // 10 values * 2 bytes each
    }

    fclose(file);
    fclose(csv_file);
}

long get_file_size(char* file_path) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Could not open file %s", file_path);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return size;
}

int twos_complement(uint16_t value, int bits){
    int val = 0;
    val = (int) value;
    if (val & (1 << (bits-1))){
        val -= 1 << bits;
    }
    return val;
}

// Convert a little endian 16 bit integer to a big endian 16 bit integer
uint16_t little_to_big(uint16_t value) {
    return htons(value);
}

int main() {
    char* file_path = "log/adxl314_test_recv_2024-06-30_12-58-09.log";
    interpret_data(file_path, 0);
    return 0;
}
