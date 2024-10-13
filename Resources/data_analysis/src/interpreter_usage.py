import os
import csv
from interpreter_interface import *

def read_calibration_data_from_csv(file_path):
    # Initialize an empty list to store BME280CalibData objects
    sensors_data = []

    # Read the CSV file
    with open(file_path, mode='r') as csv_file:
        csv_reader = csv.DictReader(csv_file)

        # Iterate over each row (each sensor's data)
        for row in csv_reader:
            # Create a BME280CalibData object for each sensor
            sensor_data = BME280CalibData(
                dig_T1=int(row['dig_T1']),
                dig_T2=int(row['dig_T2']),
                dig_T3=int(row['dig_T3']),
                dig_P1=int(row['dig_P1']),
                dig_P2=int(row['dig_P2']),
                dig_P3=int(row['dig_P3']),
                dig_P4=int(row['dig_P4']),
                dig_P5=int(row['dig_P5']),
                dig_P6=int(row['dig_P6']),
                dig_P7=int(row['dig_P7']),
                dig_P8=int(row['dig_P8']),
                dig_P9=int(row['dig_P9']),
                dig_H1=int(row['dig_H1']),
                dig_H2=int(row['dig_H2']),
                dig_H3=int(row['dig_H3']),
                dig_H4=int(row['dig_H4']),
                dig_H5=int(row['dig_H5']),
                dig_H6=int(row['dig_H6'])
            )
            sensors_data.append(sensor_data)

    return sensors_data

if __name__ == "__main__":
    # Define the CSV file path
    bme280_calib_path = "sensor_calibration/bme280_calib.csv"

    # Read the calibration data from CSV
    sensors_calibration_data = read_calibration_data_from_csv(bme280_calib_path)

    # Assuming you want to access the two sensors (row 0 and row 1 in the CSV)
    sensor_0 = sensors_calibration_data[0]
    sensor_1 = sensors_calibration_data[1]
    
    # Path to the shared library
    lib_path = os.path.join(os.path.dirname(__file__), '../lib/libdata.so')

    # Create sensor object
    interpeter = interpreter_interface(lib_path, sensor_0)

    # Example: Call convertToAccel
    raw_accel = 0x0010  # Example input value
    accel = interpeter.convert_to_accel(raw_accel)
    print(f"Acceleration: {accel}")

    # Example: Call compensate_temperature
    adc_T = 0x865d  # Example raw temperature value
    temp_data = interpeter.compensate_temperature(adc_T)
    print(f"t_fine: {temp_data.t_fine}, Temperature: {temp_data.temperature:.2f} °C")

    # Example: Call compensate_pressure
    adc_P = 0x5f18  # Example raw pressure value
    pressure = interpeter.compensate_pressure(adc_P, temp_data.t_fine)
    print(f"Pressure: {pressure} Pa")

    # Example: Call compensate_humidity
    adc_H = 0x5343  # Example raw humidity value
    humidity = interpeter.compensate_humidity(adc_H, temp_data.t_fine)
    print(f"Humidity: {humidity} %rH")