# HeartRate monitor with ESP32 and MAX30102

Code for simple HeartRate monitor with ESP32/MAX30102 and Python code for calculation.

The ESP32 configures the MAX30102 and read its data. Then send it through COM.

The Python code opens the COM, captures data and make an estimate every 5 sec.


## Conection

ESP32 | MAX30102 board
:------------: | :-------------:
3V3|VIN
GND|GND
SDA (GPIO21)|SDA
SCL (GPIO22)|SCL

