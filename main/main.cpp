#include <stdio.h>

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "driver/gpio.h"
#include "esp_rom_gpio.h"
#include "esp_intr_alloc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "max30102.h"

#define TAG "main"

#define PIN_INT GPIO_NUM_19

extern "C"
{
    void IRAM_ATTR int_isr_handler(void *args)
    {
        printf("INTERRUPT\n");
    }

    void app_main(void)
    {
        i2c_init();
        // Usage example:
        max30102_init();
        if (max30102_config() != ESP_OK)
        {
            printf("max30102_init failed\n");
        }
        else
        {
            printf("max30102_init success\n");
        }
        uint8_t value = 0;
        max30102_readRegister(MAX30102_INTENABLE1, &value, 1);
        printf("Value INT1: %d\n", value);
        max30102_readRegister(MAX30102_FIFOCONFIG, &value, 1);
        printf("Value FIFO: %d\n", value);
        max30102_readRegister(MAX30102_MODECONFIG, &value, 1);
        printf("Value MODE: %d\n", value);
        max30102_readRegister(MAX30102_PARTICLECONFIG, &value, 1);
        printf("Value SPO2: %d\n", value);
        max30102_readRegister(MAX30102_LED1_PULSEAMP, &value, 1);
        printf("Value LED1: %d\n", value);
        max30102_readRegister(MAX30102_LED2_PULSEAMP, &value, 1);
        printf("Value LED2: %d\n", value);
        max30102_readRegister(MAX30102_MULTILEDCONFIG1, &value, 1);
        printf("Value SLOT1: %d\n", value);
        max30102_readRegister(MAX30102_MULTILEDCONFIG2, &value, 1);
        printf("Value SLOT2: %d\n", value);
        uint32_t red, ir;
        max30102_clearFiFo();
        for (;;)
        {

            // float temp = max30102_readTemperature(); // Read temperature
            // printf("Temperature: %.2f\n", temp);
            // max30102_readSensor(red, ir); // Read red and ir values
            // printf("%lu\n%lu\n", red, ir);
            // max30102_readSensor(red); // Read red values
            // printf("%lu\n", red);
            max30102_readSensor(red, ir); // Read red and ir values
            printf("%lu\n", ir);
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }
    }
}