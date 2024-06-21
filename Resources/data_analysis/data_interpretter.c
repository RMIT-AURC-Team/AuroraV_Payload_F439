#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

// Line 49255 becomes no good

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
        int1 = le16toh(int1);  // Convert from little endian to host byte order

        // Accel Y
        result += fread(&int2, sizeof(int16_t), 1, file);
        int2 = le16toh(int2);


        // Accel Z
        result += fread(&int3, sizeof(int16_t), 1, file);
        int3 = le16toh(int3);


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

        // Convert int1, int2, and int3 to m/s²
        float int1_mpss = int1 * 0.012;
        float int2_mpss = int2 * 0.012;
        float int3_mpss = int3 * 0.012;

        // Write the data to the CSV file
        // fprintf(csv_file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", timestamp, int1, int2, int3, int4, int5, int6, int7, int8, int9);
        fprintf(csv_file, "%d,%f,%f,%f,%d,%d,%d,%d,%d,%d\n", timestamp, int1_mpss, int2_mpss, int3_mpss, int4, int5, int6, int7, int8, int9);

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

int main() {
    char* file_path = "log/recv_2024-06-21_16-51-06_50hz.log";
    interpret_data(file_path, 0);

    return 0;
}
