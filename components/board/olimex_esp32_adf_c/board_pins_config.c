/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2019 <ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "audio_error.h"
#include "audio_mem.h"
#include "board.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "soc/io_mux_reg.h"
#include "soc/soc_caps.h"
#include <string.h>

static const char* TAG = "OLIMEX_ESP32_ADF_REV_C";

esp_err_t get_i2c_pins(i2c_port_t port, i2c_config_t* i2c_config) {
  AUDIO_NULL_CHECK(TAG, i2c_config, return ESP_FAIL);
  if (port == I2C_NUM_0 || port == I2C_NUM_1) {
    i2c_config->sda_io_num = GPIO_NUM_18;
    i2c_config->scl_io_num = GPIO_NUM_23;
  } else {
    i2c_config->sda_io_num = -1;
    i2c_config->scl_io_num = -1;
    ESP_LOGE(TAG, "i2c port %d is not supported", port);
    return ESP_FAIL;
  }
  return ESP_OK;
}

esp_err_t get_i2s_pins(int port, board_i2s_pin_t* i2s_config) {
  AUDIO_NULL_CHECK(TAG, i2s_config, return ESP_FAIL);
  if (port == 0 || port == 1) {
    i2s_config->mck_io_num = GPIO_NUM_0;
    i2s_config->bck_io_num = GPIO_NUM_5;
    i2s_config->ws_io_num = GPIO_NUM_25;
    i2s_config->data_out_num = GPIO_NUM_26;
    i2s_config->data_in_num = GPIO_NUM_35;
  } else {
    memset(i2s_config, -1, sizeof(board_i2s_pin_t));
    ESP_LOGE(TAG, "i2s port %d is not supported", port);
    return ESP_FAIL;
  }
  return ESP_OK;
}

esp_err_t get_spi_pins(spi_bus_config_t* spi_config, spi_device_interface_config_t* spi_device_interface_config) {
  AUDIO_NULL_CHECK(TAG, spi_config, return ESP_FAIL);
  AUDIO_NULL_CHECK(TAG, spi_device_interface_config, return ESP_FAIL);

  spi_config->mosi_io_num = GPIO_NUM_13;
  spi_config->miso_io_num = GPIO_NUM_12;
  spi_config->sclk_io_num = GPIO_NUM_14;
  spi_config->quadwp_io_num = -1;
  spi_config->quadhd_io_num = -1;

  spi_device_interface_config->spics_io_num = -1;

  ESP_LOGW(TAG, "SPI interface is not supported");
  return ESP_OK;
}

// sdcard

int8_t get_sdcard_intr_gpio(void) { return SDCARD_INTR_GPIO; }

int8_t get_sdcard_open_file_num_max(void) { return SDCARD_OPEN_FILE_NUM_MAX; }

// input-output pins

int8_t get_auxin_detect_gpio(void) { return AUXIN_DETECT_GPIO; }

int8_t get_headphone_detect_gpio(void) { return HEADPHONE_DETECT; }

int8_t get_pa_enable_gpio(void) { return PA_ENABLE_GPIO; }

// button pins

int8_t get_input_rec_id(void) { return BUTTON_REC_ID; }

int8_t get_input_mode_id(void) { return BUTTON_MODE_ID; }

// touch pins

int8_t get_input_set_id(void) { return BUTTON_SET_ID; }

int8_t get_input_play_id(void) { return BUTTON_PLAY_ID; }

int8_t get_input_volup_id(void) { return BUTTON_VOLUP_ID; }

int8_t get_input_voldown_id(void) { return BUTTON_VOLDOWN_ID; }

// led pins

int8_t get_green_led_gpio(void) { return GREEN_LED_GPIO; }
