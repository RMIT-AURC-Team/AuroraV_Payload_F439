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

    FILE *csv_file = fopen("log/accel_interpretted_data4.csv", "w");
    if (csv_file == NULL) {
        printf("Could not open log/accel_interpretted_data.csv for writing");
        return;
    }

    // Write the header
    fprintf(csv_file, "X_Accel,Y_Accel,Z_Accel\n");

    fseek(file, skip_bytes, SEEK_SET);

    while (1) {
        int16_t int1, int2, int3;


        // Accel X
        size_t result = fread(&int1, sizeof(int16_t), 1, file);
        int1 = int1 & 0x1FFF;  // Mask the first 3 bits

        // Accel Y
        result += fread(&int2, sizeof(int16_t), 1, file);
        int2 = int2 & 0x1FFF;  // Mask the first 3 bits


        // Accel Z
        result += fread(&int3, sizeof(int16_t), 1, file);
        int3 = int3 & 0x1FFF;  // Mask the first 3 bits

        if (result < 3) {
            break;
        }

        // Convert int1, int2, and int3 to Gs
        // float int1_g = (float) (twos_complement(int1, 13)  * 0.049) - 0.2;
        // float int2_g = (float) (twos_complement(int2, 13) * 0.049) + 0.69;
        // float int3_g = (float) (twos_complement(int3, 13) * 0.049) + 2.37;
        float int1_g = (float) (twos_complement(int1, 13)  * 0.049);
        float int2_g = (float) (twos_complement(int2, 13) * 0.049);
        float int3_g = (float) (twos_complement(int3, 13) * 0.049);

        // Write the data to the CSV file
        fprintf(csv_file, "%f,%f,%f\n", int1_g, int2_g, int3_g);
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
    char* file_path = "log/adxl_test4.log";
    interpret_data(file_path, 0);
    return 0;
}
