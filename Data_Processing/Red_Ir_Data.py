import serial
from scipy.signal import find_peaks, butter, filtfilt
import numpy as np
import matplotlib.pyplot as plt

MODE = 1 # 0: Collect data, 1: Load data from file

NUMBER_OF_SAMPLES_PER_SECOND = 50  # Number of samples per second
TIME_PER_SAMPLE = 1 / NUMBER_OF_SAMPLES_PER_SECOND  # Time per sample in seconds
NUMBER_OF_SAMPLES = 5 * NUMBER_OF_SAMPLES_PER_SECOND  # Number of samples to collect

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

def print_peaks(data, peaks, led="led",):
  plt.figure(figsize=(10, 5))
  plt.plot(data, label="Data", color='blue')
  plt.plot(peaks, data[peaks], "ro", label="Peaks")  # Mark peaks with red dots
  plt.xlabel("Index")
  plt.ylabel("Value")
  plt.title("Detected Peaks in " + led)
  plt.legend()
  plt.show()

red_data = np.zeros(shape=(NUMBER_OF_SAMPLES, 1), dtype= int)
ir_data = np.zeros(shape=(NUMBER_OF_SAMPLES, 1), dtype= int)
counter = 0
smp = 0

if MODE == 0:

  z1serial = serial.Serial(port=z1port, baudrate=z1baudrate, timeout=1)
  while z1serial.is_open:
    data = z1serial.readline().strip()
    if data:  # Check if the string is not empty
      try:
          num = int(data)
          if(num > 100000 and num < 4000000):  # Check if the number is within the range
            print(num)
            if(counter % 2 == 0):
              red_data[round(counter/2)] = num
            else:
              ir_data[round((counter-1)/2)] = num
            counter += 1
          if counter == 2 * NUMBER_OF_SAMPLES:
              np.savetxt("red_data" + str(smp) + ".csv", red_data, delimiter=',', fmt='%d')  # Save as CSV
              np.savetxt("ir_data" + str(smp) + ".csv", ir_data, delimiter=',', fmt='%d')  # Save as CSV
              smp += 1
              counter = 0
      except ValueError:
          print("Invalid input: Cannot convert to an integer")
      except KeyboardInterrupt:
          print("Keyboard interrupt")
          break

  


if MODE == 1:
  red_data = np.loadtxt('red_data0.csv', delimiter=',')
  ir_data = np.loadtxt('ir_data0.csv', delimiter=',')

  # print_plot(red_data)

  # print_plot(ir_data)

  red_peaks = detect_peaks(-red_data, distance=15, prominence=10)
  print_peaks(red_data, red_peaks, "RED")

  ir_peaks = detect_peaks(-ir_data, distance=15, prominence=10)
  print_peaks(ir_data, ir_peaks, "IR")

  # heart_rate = 0
  # for i in range(0, len(ir_peaks)-1):
  #   heart_rate += ir_peaks[i+1] - ir_peaks[i]
  #   print(str(ir_peaks[i+1]) + " - " + str(ir_peaks[i]) + " = " + str(heart_rate))
  # print(heart_rate / (len(ir_peaks)-1))
  # print_peaks(ir_data, ir_peaks)

