import serial
import numpy as np
from func_auto_calib import func_auto

# Serial port configuration
SERIAL_PORT = '/dev/ttyACM0'  # Change this to the name of the port you're using
BAUD_RATE = 9600  # Set the baud rate used by your sensor
data_count = 1000  # Number of data points to store before calculating the average
line_counter = 1

# Connect to the serial port
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

# Dictionary to store pressure data for each sensor
sensor_data = {
    "sensor_1": [],
    "sensor_2": [],
    "sensor_3": [],
    "sensor_4": []
}

#Offset variables
sensor_offset = {
    "sensor_1": 0,
    "sensor_2": 0,
    "sensor_3": 0,
    "sensor_4": 0
}

# Function to calculate the average
def calculate_average(data):
    return np.mean(data)

# Read from the serial port and store data
while True:
    if ser.in_waiting > 0:  # Check if there is data in the buffer
        line = ser.readline().decode('latin-1').strip()  # Read and decode the line of data
        values = line.split(",")  # Split the data by commas
        try:
            sensor_data["sensor_1"].append(float(values[0]))
            sensor_data["sensor_2"].append(float(values[1]))
            sensor_data["sensor_3"].append(float(values[2]))
            sensor_data["sensor_4"].append(float(values[3]))
            print(f"Line {line_counter}: {line}")
            line_counter += 1
            print(len(sensor_data["sensor_1"]))
        except ValueError:
            print("Error: invalid data received:", line)
    if len(sensor_data["sensor_1"])== data_count: #all(len(readings) == data_count for readings in sensor_data.values()) == data_count:
        # Calculate and display the average
        averages = func_auto(sensor_data)
        for sensor, avg in averages.items():
            print(f"Average Pressure {sensor}: {avg}")
            sensor_offset[sensor] = avg
    print(sensor_offset)