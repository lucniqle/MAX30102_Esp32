import serial
from scipy.signal import find_peaks
import numpy as np
import matplotlib.pyplot as plt

NUMBER_OF_SAMPLES_PER_SECOND = 50  # Number of samples per second
TIME_PER_SAMPLE = 1 / NUMBER_OF_SAMPLES_PER_SECOND  # Time per sample in seconds
NUMBER_OF_SAMPLES = 10 * NUMBER_OF_SAMPLES_PER_SECOND  # Number of samples to collect

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

data = np.loadtxt('ir_data.csv', delimiter=',')

ir_data = moving_average(data, 7)

ir_peaks = detect_peaks(-ir_data, distance=15, prominence=10)
print(ir_peaks)
heart_rate = 0
for i in range(0, len(ir_peaks)-1):
    heart_rate += ir_peaks[i+1] - ir_peaks[i]
    print(str(ir_peaks[i+1]) + " - " + str(ir_peaks[i]) + " = " + str(heart_rate))
bps = heart_rate / (len(ir_peaks)-1)
bpm = 60 / (bps * TIME_PER_SAMPLE)
print(str(len(ir_peaks)) + " - " + str(heart_rate) + "- " + str(bps) + "- " + str(bpm))
print_peaks(ir_data, ir_peaks)
