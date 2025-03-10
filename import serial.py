import serial
import time
import numpy as np
import matplotlib.pyplot as plt

NUMBER_OF_SAMPLES = 6000  # Number of samples to collect

z1baudrate = 115200
z1port = 'COM7'  # set the correct port before run it

z1serial = serial.Serial(port=z1port, baudrate=z1baudrate, timeout=1)

raw_data = np.zeros(shape=(NUMBER_OF_SAMPLES, 1), dtype= int)
counter = 0

while z1serial.is_open:
  data = z1serial.readline().strip()
  if data:  # Check if the string is not empty
    try:
        num = int(data)
        if(num > 50000):  # Check if the number is within the range
          print(num)
          raw_data[counter] = num
          counter += 1
        if counter == NUMBER_OF_SAMPLES:
            z1serial.close()
            break
    except ValueError:
        print("Invalid input: Cannot convert to an integer")

np.savetxt('array.csv', raw_data, delimiter=',', fmt='%d')  # Save as CSV

# print(raw_data)

# plt.plot(raw_data)
# plt.xlabel('samples')
# plt.ylabel('reading')
# plt.axis('tight')
# plt.show()