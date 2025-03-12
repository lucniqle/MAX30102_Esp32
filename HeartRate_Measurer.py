import serial
from scipy.signal import find_peaks
import matplotlib.pyplot as plt
import numpy as np

NUMBER_OF_SAMPLES_PER_SECOND = 50  # Number of samples per second
TIME_PER_SAMPLE = 1 / NUMBER_OF_SAMPLES_PER_SECOND  # Time per sample in seconds
NUMBER_OF_SAMPLES = 5 * NUMBER_OF_SAMPLES_PER_SECOND  # Number of samples to collect

z1baudrate = 115200
z1port = 'COM7'  # set the correct port before run it


def print_plot(data):
  plt.plot(data)
  plt.xlabel('samples')
  plt.ylabel('reading')
  plt.axis('tight')
  plt.show()

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

def moving_average(a, n=3):
    ret = np.cumsum(a, dtype=float)
    ret[n:] = ret[n:] - ret[:-n]
    return ret[n - 1:] / n

z1serial = serial.Serial(port=z1port, baudrate=z1baudrate, timeout=1)

raw_data = np.zeros(shape=(NUMBER_OF_SAMPLES), dtype= int)
pp_data = np.zeros(shape=(NUMBER_OF_SAMPLES), dtype= int)
counter = 0
smp = 0

while z1serial.is_open:
    data = z1serial.readline().strip()
    if data:  # Check if the string is not empty
        try:
            num = int(data)
            if(num > 100000 and num < 4000000):  # Check if the number is within the range
                if(counter < NUMBER_OF_SAMPLES):
                    raw_data[counter] = num
                    counter += 1
                else:
                    counter = 0
                    pp_data = moving_average(raw_data, 8)
                    peaks = detect_peaks(-pp_data, distance=15, prominence=10)
                    heart_rate = 0
                    for i in range(0, len(peaks)-1):
                        heart_rate += peaks[i+1] - peaks[i]
                    if((len(peaks)-1) != 0):
                        # print_plot(ir_data)
                        bps = heart_rate / (len(peaks)-1)
                        bpm = 60 / (bps * TIME_PER_SAMPLE)
                        print(str(peaks) + " - " + str(heart_rate) + "- " + str(bpm))
        except KeyboardInterrupt:
            print("Exception")
            z1serial.close()


  


