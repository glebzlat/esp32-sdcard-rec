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

#ifndef _AUDIO_BOARD_DEFINITION_H_
#define _AUDIO_BOARD_DEFINITION_H_

#include "driver/touch_pad.h"

/**
 * @brief SDCARD Function Definition
 */
#define FUNC_SDCARD_EN (1)
#define SDCARD_OPEN_FILE_NUM_MAX 5
#define SDCARD_INTR_GPIO -1 // GPIO_NUM_34

#define ESP_SD_PIN_CS GPIO_NUM_21
#define ESP_SD_PIN_CLK GPIO_NUM_14
#define ESP_SD_PIN_CMD GPIO_NUM_13
#define ESP_SD_PIN_D0 GPIO_NUM_12
#define ESP_SD_PIN_D1 -1 // GPIO_NUM_4
#define ESP_SD_PIN_D2 -1 // GPIO_NUM_12
#define ESP_SD_PIN_D3 ESP_SD_PIN_CS

/**
 * @brief LED Function Definition
 */
#define FUNC_SYS_LED_EN (1)
#define GREEN_LED_GPIO GPIO_NUM_2

/**
 * @brief Audio Codec Chip Function Definition
 */
#define FUNC_AUDIO_CODEC_EN (1)
#define AUXIN_DETECT_GPIO -1
#define HEADPHONE_DETECT -1
#define PA_ENABLE_GPIO -1
#define CODEC_ADC_I2S_PORT ((i2s_port_t)0)
#define CODEC_ADC_BITS_PER_SAMPLE ((i2s_data_bit_width_t)16) /* 16bit */
#define CODEC_ADC_SAMPLE_RATE (48000)
#define RECORD_HARDWARE_AEC (false)
#define BOARD_PA_GAIN (10) /* Power amplifier gain defined by board (dB) */

/**
 * @brief ADC input data format
 */
#define AUDIO_ADC_INPUT_CH_FORMAT "M"

extern audio_hal_func_t AUDIO_CODEC_ES8388_CUSTOM_HANDLE;
#define AUDIO_CODEC_DEFAULT_CONFIG()                                                                                   \
  {                                                                                                                    \
      .adc_input = AUDIO_HAL_ADC_INPUT_LINE1,                                                                          \
      .dac_output = AUDIO_HAL_DAC_OUTPUT_ALL,                                                                          \
      .codec_mode = AUDIO_HAL_CODEC_MODE_BOTH,                                                                         \
      .i2s_iface =                                                                                                     \
          {                                                                                                            \
              .mode = AUDIO_HAL_MODE_SLAVE,                                                                            \
              .fmt = AUDIO_HAL_I2S_NORMAL,                                                                             \
              .samples = AUDIO_HAL_48K_SAMPLES,                                                                        \
              .bits = AUDIO_HAL_BIT_LENGTH_16BITS,                                                                     \
          },                                                                                                           \
  };

#define SPI_MOSI_IO GPIO_NUM_13
#define SPI_MISO_IO GPIO_NUM_12
#define SPI_SCK_IO GPIO_NUM_14

#define ILI9341_DC_IO GPIO_NUM_12
#define DISPLAY_SPI_CS_IO GPIO_NUM_15
#define DISPLAY_SPI_MOSI_IO GPIO_NUM_13
#define DISPLAY_SPI_CLK_IO GPIO_NUM_14

/**
 * @brief Button Function Definition
 */
#define FUNC_BUTTON_EN (1)
#define INPUT_KEY_NUM 6
#define BUTTON_REC_ID GPIO_NUM_36
#define BUTTON_MODE_ID GPIO_NUM_39
#define BUTTON_SET_ID TOUCH_PAD_NUM9     /* GPIO 32 */
#define BUTTON_PLAY_ID TOUCH_PAD_NUM8    /* GPIO 33 */
#define BUTTON_VOLUP_ID TOUCH_PAD_NUM7   /* GPIO 27 */
#define BUTTON_VOLDOWN_ID TOUCH_PAD_NUM0 /* GPIO 4 */

/*
 * See "esp-adf/components/esp_peripherals/include/periph_touch.h"
 * and "esp-adf/components/esp_peripherals/lib/touch/touch.c".
 *
 * Selection bits shifted by the number corresponding to touch pad number.
 * E.g. TOUCH_PAD_SEL9 = 1 << 9. On initialization these masks decoded back
 * to touch pad numbers by unshifting.
 */
#define TOUCH_MASK (TOUCH_PAD_SEL0 | TOUCH_PAD_SEL7 | TOUCH_PAD_SEL8 | TOUCH_PAD_SEL9)

#define INPUT_KEY_DEFAULT_INFO()                                                                                       \
  {                                                                                                                    \
      {                                                                                                                \
          .type = PERIPH_ID_BUTTON,                                                                                    \
          .user_id = INPUT_KEY_USER_ID_REC,                                                                            \
          .act_id = BUTTON_REC_ID,                                                                                     \
      },                                                                                                               \
      {                                                                                                                \
          .type = PERIPH_ID_BUTTON,                                                                                    \
          .user_id = INPUT_KEY_USER_ID_MODE,                                                                           \
          .act_id = BUTTON_MODE_ID,                                                                                    \
      },                                                                                                               \
      {                                                                                                                \
          .type = PERIPH_ID_TOUCH,                                                                                     \
          .user_id = INPUT_KEY_USER_ID_SET,                                                                            \
          .act_id = BUTTON_SET_ID,                                                                                     \
      },                                                                                                               \
      {                                                                                                                \
          .type = PERIPH_ID_TOUCH,                                                                                     \
          .user_id = INPUT_KEY_USER_ID_PLAY,                                                                           \
          .act_id = BUTTON_PLAY_ID,                                                                                    \
      },                                                                                                               \
      {                                                                                                                \
          .type = PERIPH_ID_TOUCH,                                                                                     \
          .user_id = INPUT_KEY_USER_ID_VOLUP,                                                                          \
          .act_id = BUTTON_VOLUP_ID,                                                                                   \
      },                                                                                                               \
      {                                                                                                                \
          .type = PERIPH_ID_TOUCH,                                                                                     \
          .user_id = INPUT_KEY_USER_ID_VOLDOWN,                                                                        \
          .act_id = BUTTON_VOLDOWN_ID,                                                                                 \
      },                                                                                                               \
  }

#endif
