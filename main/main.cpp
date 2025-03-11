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
        gpio_install_isr_service(0);
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
        uint8_t counter = 0;
        // esp_rom_gpio_pad_select_gpio(PIN_INT);
        // ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_set_direction(PIN_INT, GPIO_MODE_INPUT));
        // ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_set_intr_type(PIN_INT, GPIO_INTR_ANYEDGE));
        // ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_isr_handler_add(PIN_INT, int_isr_handler, NULL));
        for (;;)
        {
            // if (counter > 40)
            //     break;
            // max30102_readRegister(MAX30102_FIFOREADPTR, &value, 1);
            // printf("READ: %d\n", value);
            // max30102_readRegister(MAX30102_FIFOWRITEPTR, &value, 1);
            // printf("WRITE: %d\n", value);
            // float temp = max30102_readTemperature();
            // printf("Temperature: %.2f\n", temp);
            max30102_readSensor(red, ir);
            // printf("Red: %lu, IR: %lu\n", red, ir);
            // printf("%lu\n%lu\n", red, ir);
            printf("%lu\n", ir);
            // counter++;
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }
    }
}