#include "esp_log_level.h"
#include <audio_mem.h>
#include <audio_pipeline.h>
#include <esp_log.h>
#include <esp_peripherals.h>
#include <freertos/idf_additions.h>
#include <i2s_stream.h>
#include <input_key.h>
#include <input_key_service.h>
#include <periph_sdcard.h>
#include <pipeline.h>
#include <portmacro.h>

#include <board.h>

typedef enum {
  STATE_STOP = 0,
  STATE_RECORD_AUDIO,
  STATE_PLAY_AUDIO,
} program_state;

typedef struct {
  audio_pipeline_handle_t pipeline;
  program_state state;
} context_struct;

static void play_cb(void* ctx);
static void record_cb(void* ctx);
static void set_volume_cb(int volume, void* ctx);

static char const* TAG = "PLAYER";

void app_main(void) {
  esp_log_level_set("*", ESP_LOG_INFO);

  ESP_LOGI(TAG, "Initialize peripherals management");
  esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
  esp_periph_set_handle_t periph_set = esp_periph_set_init(&periph_cfg);

  ESP_LOGI(TAG, "Initialize and start peripherals");
  audio_board_key_init(periph_set);

  ESP_LOGI(TAG, "Mount SDCard");
  while (audio_board_sdcard_init(periph_set, SD_MODE_SPI) != ESP_OK) {
    ESP_LOGW(TAG, "Install SD Card");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  ESP_LOGI(TAG, "Start audio codec");
  audio_board_handle_t board_handle = audio_board_init();
  audio_hal_ctrl_codec(board_handle->audio_hal, AUDIO_HAL_CODEC_MODE_BOTH, AUDIO_HAL_CTRL_START);

  ESP_LOGI(TAG, "Setup input key service");
  context_struct program_ctx = {
      .pipeline = NULL,
      .state = STATE_STOP,
  };
  input_key_service_config input_key_service_cfg = {
      .periph_set = periph_set,
      .play_cb = play_cb,
      .record_cb = record_cb,
      .volume_set_cb = set_volume_cb,
      .ctx = &program_ctx,
  };
  periph_service_handle_t input_key_service = create_input_key_service(&input_key_service_cfg);

  while (1) {
    AUDIO_MEM_SHOW(TAG);
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }

  ESP_LOGI(TAG, "Destroy input key service");
  destroy_input_key_service(input_key_service);
}

static void play_cb(void* ctx) {
  context_struct* cb_ctx = ctx;
  if (cb_ctx->state == STATE_RECORD_AUDIO) {
    ESP_LOGI(TAG, "State change: record -> play");
    destroy_recording_pipeline(cb_ctx->pipeline);
    vTaskDelay(150 / portTICK_PERIOD_MS);
  }
  ESP_LOGI(TAG, "Playback");
  cb_ctx->pipeline = setup_playback_pipeline();
  cb_ctx->state = STATE_PLAY_AUDIO;
}

static void record_cb(void* ctx) {
  context_struct* cb_ctx = ctx;
  if (cb_ctx->state == STATE_PLAY_AUDIO) {
    ESP_LOGI(TAG, "State change: play -> record");
    destroy_playback_pipeline(cb_ctx->pipeline);
    vTaskDelay(150 / portTICK_PERIOD_MS);
  }
  ESP_LOGI(TAG, "Recording");
  cb_ctx->pipeline = setup_recording_pipeline();
  cb_ctx->state = STATE_RECORD_AUDIO;
}

static void set_volume_cb(int volume, void* ctx) {
  ESP_LOGI(TAG, "Set playback volume: %d", volume);
  context_struct* cb_ctx = ctx;
  if (cb_ctx->state == STATE_PLAY_AUDIO) {
    audio_element_handle_t i2s_writer = audio_pipeline_get_el_by_tag(cb_ctx->pipeline, "i2s");
    i2s_alc_volume_set(i2s_writer, volume);
  }
}
