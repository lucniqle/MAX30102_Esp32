#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "max30102.h"

#define TAG "max30102"

void i2c_init(void)
{
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

esp_err_t max30102_init(void)
{
    ESP_LOGI(TAG, "max30102_init");
    esp_err_t error = ESP_ERR_TIMEOUT;

    // read data from sensor
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MAX30102_ADDRESS << 1) | I2C_MASTER_WRITE, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    error = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return error;
}

esp_err_t max30102_config(void)
{
    ESP_LOGI(TAG, "max30102_config");
    esp_err_t error = ESP_ERR_TIMEOUT;

    max30102_reset();
    vTaskDelay(100 / portTICK_PERIOD_MS);
    // error = max30102_writeRegister(MAX30102_INTENABLE1, MAX30102_INT_DATA_RDY_ENABLE);
    // if (error != ESP_OK)
    // {
    //     return error;
    // }
    error = max30102_writeRegister(MAX30102_FIFOCONFIG, ((MAX30102_ROLLOVER_ENABLE << 4)) | (MAX30102_SAMPLEAVG_4));
    if (error != ESP_OK)
    {
        return error;
    }
    error = max30102_writeRegister(MAX30102_MODECONFIG, MAX30102_MODE_MULTILED);
    if (error != ESP_OK)
    {
        return error;
    }
    error = max30102_writeRegister(MAX30102_PARTICLECONFIG, ((MAX30102_ADCRANGE_4096 & ~MAX30102_ADCRANGE_MASK) | (MAX30102_SAMPLERATE_200 & ~MAX30102_SAMPLERATE_MASK) | MAX30102_PULSEWIDTH_411));
    if (error != ESP_OK)
    {
        return error;
    }
    error = max30102_writeRegister(MAX30102_LED1_PULSEAMP, MAX30102_LEDCURR_LED1);
    if (error != ESP_OK)
    {
        return error;
    }
    error = max30102_writeRegister(MAX30102_LED2_PULSEAMP, MAX30102_LEDCURR_LED2);
    if (error != ESP_OK)
    {
        return error;
    }
    error = max30102_writeRegister(MAX30102_MULTILEDCONFIG1, (SLOT_RED_LED & ~MAX30102_SLOT1_MASK) | ((SLOT_IR_LED << 4) & ~MAX30102_SLOT2_MASK));
    if (error != ESP_OK)
    {
        return error;
    }
    error = max30102_writeRegister(MAX30102_MULTILEDCONFIG2, 0);
    if (error != ESP_OK)
    {
        return error;
    }
    error = max30102_clearFiFo();

    return error;
}

esp_err_t max30102_writeRegister(uint8_t reg, uint8_t value)
{
    esp_err_t error = ESP_ERR_TIMEOUT;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MAX30102_ADDRESS << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, reg, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, value, I2C_MASTER_ACK);
    i2c_master_stop(cmd);
    error = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return error;
}

esp_err_t max30102_readRegister(uint8_t reg, uint8_t *data, uint8_t size)
{
    esp_err_t error = ESP_ERR_TIMEOUT;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MAX30102_ADDRESS << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, reg, I2C_MASTER_ACK);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MAX30102_ADDRESS << 1) | I2C_MASTER_READ, I2C_MASTER_ACK);
    i2c_master_read(cmd, data, size, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    error = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return error;
}

esp_err_t max30102_readSensor(uint32_t &red, uint32_t &ir)
{
    esp_err_t error = ESP_ERR_TIMEOUT;
    uint8_t data[6];
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MAX30102_ADDRESS << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, 0x07, I2C_MASTER_ACK);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MAX30102_ADDRESS << 1) | I2C_MASTER_READ, I2C_MASTER_ACK);
    i2c_master_read(cmd, data, sizeof(data), I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    error = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    red = ((uint32_t)data[0] << 16) | ((uint32_t)data[1] << 8) | data[2];
    ir = ((uint32_t)data[3] << 16) | ((uint32_t)data[4] << 8) | data[5];

    return error;
}

esp_err_t max30102_reset(void)
{
    return max30102_writeRegister(MAX30102_MODECONFIG, MAX30102_RESET);
}

esp_err_t max30102_shutDown(void)
{
    return max30102_writeRegister(MAX30102_MODECONFIG, MAX30102_SHUTDOWN);
}

esp_err_t max30102_wakeUp(void)
{
    return max30102_writeRegister(MAX30102_MODECONFIG, MAX30102_WAKEUP);
}

esp_err_t max30102_clearFiFo(void)
{
    esp_err_t error = ESP_ERR_TIMEOUT;
    error = max30102_writeRegister(MAX30102_FIFOWRITEPTR, 0);
    if (error != ESP_OK)
    {
        return error;
    }
    error = max30102_writeRegister(MAX30102_FIFOREADPTR, 0);
    if (error != ESP_OK)
    {
        return error;
    }
    error = max30102_writeRegister(MAX30102_FIFOOVERFLOW, 0);
    if (error != ESP_OK)
    {
        return error;
    }
    return error;
}

float max30102_readTemperature(void)
{
    uint8_t tint = 0;
    uint8_t tfrac = 0;
    max30102_writeRegister(MAX30102_DIETEMPCONFIG, 0x01);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    max30102_readRegister(MAX30102_DIETEMPINT, &tint, 1);
    max30102_readRegister(MAX30102_DIETEMPFRAC, &tfrac, 1);
    return tint + ((tfrac & 0x0F) * 0.0625);
}
