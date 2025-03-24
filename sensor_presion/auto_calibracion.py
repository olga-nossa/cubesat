import numpy as np

# Offset variables initialized to zero
offset_sensor_1 = 0
offset_sensor_2 = 0
offset_sensor_3 = 0
offset_sensor_4 = 0

# Function to calculate the average
def calculate_average(sensor_data):
    """
    Calculates the average of each sensor's readings.

    Parameters:
    sensor_data (dict): A dictionary with each sensor's readings.

    Returns:
    dict: A dictionary with the average for each sensor.
    """
    averages = {sensor: np.mean(readings) for sensor, readings in sensor_data.items()}
    return averages

#Run in main function
global offset_sensor_1, offset_sensor_2, offset_sensor_3, offset_sensor_4  # Allow modification of offset variables
sensor_data = {"sensor_1": [], "sensor_2": [], "sensor_3": [], "sensor_4": []}

# Add new readings to each sensor
for sensor, reading in new_readings.items():
    sensor_data[sensor].append(reading)

# Check if the required number of measurements has been reached
if all(len(readings) >= 1000 for readings in sensor_data.values()):
    # Calculate the average and update offsets
    averages = calculate_average(sensor_data)
    offset_sensor_1 = averages["sensor_1"]
    offset_sensor_2 = averages["sensor_2"]
    offset_sensor_3 = averages["sensor_3"]
    offset_sensor_4 = averages["sensor_4"]

#__________________________________________________________________

# Main function
def run_calibration():
    global offset_sensor_1, offset_sensor_2, offset_sensor_3, offset_sensor_4  # Allow modification of offset variables
    sensor_data = {"sensor_1": [], "sensor_2": [], "sensor_3": [], "sensor_4": []}
    calibration_done = False

    while not calibration_done:
        new_readings = {
            "sensor_1": np.random.random(),
            "sensor_2": np.random.random(),
            "sensor_3": np.random.random(),
            "sensor_4": np.random.random()
        }
        
        # Add new readings to each sensor
        for sensor, reading in new_readings.items():
            sensor_data[sensor].append(reading)
        
        # Check if the required number of measurements has been reached
        if all(len(readings) >= 1000 for readings in sensor_data.values()):
            # Calculate the average and update offsets
            averages = calculate_average(sensor_data)
            offset_sensor_1 = averages["sensor_1"]
            offset_sensor_2 = averages["sensor_2"]
            offset_sensor_3 = averages["sensor_3"]
            offset_sensor_4 = averages["sensor_4"]
            calibration_done = True

    # Print the final offsets
    print("Calculated offsets:", offset_sensor_1, offset_sensor_2, offset_sensor_3, offset_sensor_4)

# Run the main function
run_calibration()
