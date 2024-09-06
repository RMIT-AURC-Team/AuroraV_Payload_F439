import pandas as pd
import matplotlib.pyplot as plt
import os

def clear_leading_data(df):
    # Detect where the timer is reset by finding the first negative difference
    reset_indices = df[df['Timestamp'].diff() < 0].index

    df_cleaned = df
    if not reset_indices.empty:
        if(reset_indices[0] < 255):
            # Drop rows before the timer reset
            reset_index = reset_indices[0]
            df_cleaned = df.iloc[reset_index:].reset_index(drop=True)
    
    return df_cleaned

def correct_timestamps(df):
    # Correct the timestamp by detecting wraparound and adjusting accordingly
    max_timestamp_value = 65535  # Maximum value before wraparound

    # Create a corrected timestamp column
    df['Corrected_Timestamp'] = df['Timestamp'].copy()

    # Iterate over the DataFrame and correct the timestamps
    correction = 0
    for i in range(1, len(df)):
        if df.loc[i, 'Timestamp'] < df.loc[i - 1, 'Timestamp']:
            # Detected wraparound
            correction += max_timestamp_value
        df.loc[i, 'Corrected_Timestamp'] += correction
    
    return df


# Define plotting functions
def plot_acceleration_vs_time(df, output_dir):
    plt.figure(figsize=(10, 6))
    plt.plot(df['Corrected_Timestamp'], df['X_Accel'], label='X Accel')
    plt.plot(df['Corrected_Timestamp'], df['Y_Accel'], label='Y Accel')
    plt.plot(df['Corrected_Timestamp'], df['Z_Accel'], label='Z Accel')
    plt.xlabel('Timestamp')
    plt.ylabel('Acceleration (Gs)')
    plt.title('Acceleration on X, Y, and Z vs Time')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(output_dir, 'acceleration_vs_time.png'))
    plt.close()

def plot_temperature_vs_time(df, output_dir):
    plt.figure(figsize=(10, 6))
    plt.plot(df['Corrected_Timestamp'], df['Temperature0'], label='Temperature 0')
    plt.plot(df['Corrected_Timestamp'], df['Temperature1'], label='Temperature 1')
    plt.xlabel('Timestamp')
    plt.ylabel('Temperature (°C)')
    plt.title('Temperature 0 and Temperature 1 vs Time')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(output_dir, 'temperature_vs_time.png'))
    plt.close()

def plot_humidity_vs_time(df, output_dir):
    plt.figure(figsize=(10, 6))
    plt.plot(df['Corrected_Timestamp'], df['Humidity0'], label='Humidity 0')
    plt.plot(df['Corrected_Timestamp'], df['Humidity1'], label='Humidity 1')
    plt.xlabel('Timestamp')
    plt.ylabel('Humidity (%)')
    plt.title('Humidity 0 and Humidity 1 vs Time')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(output_dir, 'humidity_vs_time.png'))
    plt.close()

def plot_pressure_vs_time(df, output_dir):
    plt.figure(figsize=(10, 6))
    plt.plot(df['Corrected_Timestamp'], df['Pressure0'], label='Pressure 0')
    plt.plot(df['Corrected_Timestamp'], df['Pressure1'], label='Pressure 1')
    plt.xlabel('Timestamp')
    plt.ylabel('Pressure (Pa)')
    plt.title('Pressure 0 and Pressure 1 vs Time')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(output_dir, 'pressure_vs_time.png'))
    plt.close()


def main():
    # Load the data into a pandas DataFrame from the CSV file
    csv_file_path = "graphs/output.csv"
    df = pd.read_csv(csv_file_path)
    
    df = clear_leading_data(df)
    df = correct_timestamps(df)

    output_dir = "graphs"
    
    # Call the plotting functions
    plot_acceleration_vs_time(df, output_dir)
    plot_temperature_vs_time(df, output_dir)
    plot_humidity_vs_time(df, output_dir)
    plot_pressure_vs_time(df, output_dir)
    
if __name__ == "__main__":
    main()
