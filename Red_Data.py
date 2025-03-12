import serial
from scipy.signal import find_peaks, butter, filtfilt
import numpy as np
import matplotlib.pyplot as plt

MODE = 1 # 0: Collect data, 1: Load data from file

NUMBER_OF_SAMPLES_PER_SECOND = 50  # Number of samples per second
TIME_PER_SAMPLE = 1 / NUMBER_OF_SAMPLES_PER_SECOND  # Time per sample in seconds
NUMBER_OF_SAMPLES = 10 * NUMBER_OF_SAMPLES_PER_SECOND  # Number of samples to collect

z1baudrate = 115200
z1port = 'COM7'  # set the correct port before run it

# Function to detect peaks
def detect_peaks(data, distance=5, prominence=10):
    """
    Detect peaks in a given 1D numerical data series.

    Parameters:
    - data: 1D NumPy array of numerical values
    - distance: Minimum number of points between peaks
    - prominence: Minimum prominence of peaks

    Returns:
    - peaks: Indices of detected peaks
    """
    peaks, _ = find_peaks(data, distance=distance, prominence=prominence)
    return peaks

def print_plot(data):
  plt.plot(data)
  plt.xlabel('samples')
  plt.ylabel('reading')
  plt.axis('tight')
  plt.show()

def print_peaks(data, peaks):
  plt.figure(figsize=(10, 5))
  plt.plot(data, label="Data", color='blue')
  plt.plot(peaks, data[peaks], "ro", label="Peaks")  # Mark peaks with red dots
  plt.xlabel("Index")
  plt.ylabel("Value")
  plt.title("Detected Peaks in the Data")
  plt.legend()
  plt.show()

raw_data = np.zeros(shape=(NUMBER_OF_SAMPLES), dtype= int)
red_data = np.zeros(shape=(NUMBER_OF_SAMPLES), dtype= int)
counter = 0

z1serial = serial.Serial(port=z1port, baudrate=z1baudrate, timeout=1)
while z1serial.is_open:
    data = z1serial.readline().strip()
    if data:  # Check if the string is not empty
        try:
            num = int(data)
            if(num > 100000 and num < 4000000):  # Check if the number is within the range
                print(num)
                raw_data[counter] = num
            counter += 1
            if counter == NUMBER_OF_SAMPLES:
                z1serial.close()
                break
        except ValueError:
            print("Invalid input: Cannot convert to an integer")

np.savetxt('red_data.csv', raw_data, delimiter=',', fmt='%d')  # Save as CSV

def moving_average(a, n=3):
    ret = np.cumsum(a, dtype=float)
    ret[n:] = ret[n:] - ret[:-n]
    return ret[n - 1:] / n

red_data = moving_average(raw_data, 8)

red_peaks = detect_peaks(-red_data, distance=15, prominence=10)
print(red_peaks)
heart_rate = 0
for i in range(0, len(red_peaks)-1):
    heart_rate += red_peaks[i+1] - red_peaks[i]
    print(str(red_peaks[i+1]) + " - " + str(red_peaks[i]) + " = " + str(heart_rate))
bps = heart_rate / (len(red_peaks)-1)
bpm = 60 / (bps * TIME_PER_SAMPLE)
print(str(len(red_peaks)) + " - " + str(heart_rate) + "- " + str(bps) + "- " + str(bpm))
print_peaks(red_data, red_peaks)