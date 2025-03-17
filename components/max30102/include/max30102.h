#ifndef MAX30102_H
#define MAX30102_H

// Based on the repository:
// https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/tree/master

// Include necessary standard libraries
#include <stdint.h>

#define MAX30102_ADDRESS 0x57
#define I2C_MASTER_SCL_IO 22 // Set your SCL pin]]
#define I2C_MASTER_SDA_IO 21 // Set your SDA pin
#define I2C_MASTER_FREQ_HZ 400000
#define I2C_MASTER_NUM I2C_NUM_0

// Status Registers
#define MAX30102_INTSTAT1 0x00
#define MAX30102_INTSTAT2 0x01
#define MAX30102_INTENABLE1 0x02
#define MAX30102_INTENABLE2 0x03

// FIFO Registers
#define MAX30102_FIFOWRITEPTR 0x04
#define MAX30102_FIFOOVERFLOW 0x05
#define MAX30102_FIFOREADPTR 0x06
#define MAX30102_FIFODATA 0x07

// Configuration Registers
#define MAX30102_FIFOCONFIG 0x08
#define MAX30102_MODECONFIG 0x09
#define MAX30102_PARTICLECONFIG 0x0A // Note, sometimes listed as "SPO2" config in datasheet (pg. 11)
#define MAX30102_LED1_PULSEAMP 0x0C
#define MAX30102_LED2_PULSEAMP 0x0D
#define MAX30102_LED3_PULSEAMP 0x0E
#define MAX30102_LED_PROX_AMP 0x10
#define MAX30102_MULTILEDCONFIG1 0x11
#define MAX30102_MULTILEDCONFIG2 0x12

// Die Temperature Registers
#define MAX30102_DIETEMPINT 0x1F
#define MAX30102_DIETEMPFRAC 0x20
#define MAX30102_DIETEMPCONFIG 0x21

// Part ID Registers
#define MAX30102_REVISIONID 0xFE
#define MAX30102_PARTID 0xFF // Should always be 0x15. Identical to MAX30102.

// MAX30102 Commands
// Interrupt configuration (pg 13, 14)
#define MAX30102_INT_A_FULL_MASK ~0b10000000
#define MAX30102_INT_A_FULL_ENABLE 0x80
#define MAX30102_INT_A_FULL_DISABLE 0x00

#define MAX30102_INT_DATA_RDY_MASK ~0b01000000
#define MAX30102_INT_DATA_RDY_ENABLE 0x40
#define MAX30102_INT_DATA_RDY_DISABLE 0x00

#define MAX30102_INT_ALC_OVF_MASK ~0b00100000
#define MAX30102_INT_ALC_OVF_ENABLE 0x20
#define MAX30102_INT_ALC_OVF_DISABLE 0x00

#define MAX30102_INT_PROX_INT_MASK ~0b00010000
#define MAX30102_INT_PROX_INT_ENABLE 0x10
#define MAX30102_INT_PROX_INT_DISABLE 0x00

#define MAX30102_INT_DIE_TEMP_RDY_MASK ~0b00000010
#define MAX30102_INT_DIE_TEMP_RDY_ENABLE 0x02
#define MAX30102_INT_DIE_TEMP_RDY_DISABLE 0x00

#define MAX30102_SAMPLEAVG_MASK ~0b11100000
#define MAX30102_SAMPLEAVG_1 0x00
#define MAX30102_SAMPLEAVG_2 0x20
#define MAX30102_SAMPLEAVG_4 0x40
#define MAX30102_SAMPLEAVG_8 0x60
#define MAX30102_SAMPLEAVG_16 0x80
#define MAX30102_SAMPLEAVG_32 0xA0

#define MAX30102_ROLLOVER_MASK 0xEF
#define MAX30102_ROLLOVER_ENABLE 0x01
#define MAX30102_ROLLOVER_DISABLE 0x00

#define MAX30102_A_FULL_MASK 0xF0

// Mode configuration commands (page 19)
#define MAX30102_SHUTDOWN_MASK 0x7F
#define MAX30102_SHUTDOWN 0x80
#define MAX30102_WAKEUP 0x00

#define MAX30102_RESET_MASK 0xBF
#define MAX30102_RESET 0x40

#define MAX30102_MODE_MASK 0xF8
#define MAX30102_MODE_REDONLY 0x02
#define MAX30102_MODE_REDIRONLY 0x03
#define MAX30102_MODE_MULTILED 0x07

// Particle sensing configuration commands (pgs 19-20)
#define MAX30102_ADCRANGE_MASK 0x9F
#define MAX30102_ADCRANGE_2048 0x00
#define MAX30102_ADCRANGE_4096 0x20
#define MAX30102_ADCRANGE_8192 0x40
#define MAX30102_ADCRANGE_16384 0x60

#define MAX30102_SAMPLERATE_MASK 0xE3
#define MAX30102_SAMPLERATE_50 0x00
#define MAX30102_SAMPLERATE_100 0x04
#define MAX30102_SAMPLERATE_200 0x08
#define MAX30102_SAMPLERATE_400 0x0C
#define MAX30102_SAMPLERATE_800 0x10
#define MAX30102_SAMPLERATE_1000 0x14
#define MAX30102_SAMPLERATE_1600 0x18
#define MAX30102_SAMPLERATE_3200 0x1C

#define MAX30102_PULSEWIDTH_MASK 0xFC
#define MAX30102_PULSEWIDTH_69 0x00
#define MAX30102_PULSEWIDTH_118 0x01
#define MAX30102_PULSEWIDTH_215 0x02
#define MAX30102_PULSEWIDTH_411 0x03

// LED Current Mode configuration (pg 20)
#define MAX30102_LEDCURR_LED1 0x3F
#define MAX30102_LEDCURR_LED2 0x3F

// Multi-LED Mode configuration (pg 22)
#define MAX30102_SLOT1_MASK 0xF8
#define MAX30102_SLOT2_MASK 0x8F
#define MAX30102_SLOT3_MASK 0xF8
#define MAX30102_SLOT4_MASK 0x8F

#define SLOT_NONE 0x00
#define SLOT_RED_LED 0x01
#define SLOT_IR_LED 0x02

void i2c_init(void);

esp_err_t max30102_init(void);

esp_err_t max30102_config(void);

esp_err_t max30102_readSensor(uint32_t &red);

esp_err_t max30102_readSensor(uint32_t &red, uint32_t &ir);

esp_err_t max30102_writeRegister(uint8_t reg, uint8_t value);

esp_err_t max30102_readRegister(uint8_t reg, uint8_t *data, uint8_t size);

esp_err_t max30102_reset(void);

esp_err_t max30102_shutDown(void);

esp_err_t max30102_wakeUp(void);

esp_err_t max30102_clearFiFo(void);

float max30102_readTemperature(void);

#endif // MAX30102_H