#include <esp_log.h>
#include <periph_service.h>
#include <esp_err.h>

#include <board.h>

#include "input_key.h"

typedef struct {
  input_key_service_config* config;
  int volume;
} context_struct;

static char const* TAG = "INPUT_KEY";

static context_struct context;

static esp_err_t input_key_service_cb(periph_service_handle_t handle, periph_service_event_t* evt, void* ctx);

periph_service_handle_t create_input_key_service(input_key_service_config* config) {
  ESP_LOGI(TAG, "Create input key service");

  input_key_service_cfg_t cfg = INPUT_KEY_SERVICE_DEFAULT_CONFIG();
  cfg.handle = config->periph_set;
  cfg.based_cfg.task_stack = 4 * 1024;
  cfg.based_cfg.extern_stack = true;

  periph_service_handle_t service = input_key_service_create(&cfg);
  assert(service != NULL);

  input_key_service_info_t input_key_info[] = INPUT_KEY_DEFAULT_INFO();
  input_key_service_add_key(service, input_key_info, INPUT_KEY_NUM);

  context.volume = 100;
  context.config = config;

  ESP_ERROR_CHECK(periph_service_set_callback(service, input_key_service_cb, &context));

  // An idea to pass the context as a user data was neat, but it seems it breaks something
  // in ESP-IDF code. So I had no choice but to use global static variables:(
  // ESP_ERROR_CHECK(periph_service_set_data(service, ctx));

  return service;
}

void destroy_input_key_service(periph_service_handle_t input_service) {
  ESP_ERROR_CHECK(periph_service_destroy(input_service));
}

static inline void context_volup(context_struct* ctx, int val) {
  ctx->volume += val;
  if (ctx->volume > 100) {
    ctx->volume = 100;
  }
}

static inline void context_voldown(context_struct* ctx, int val) {
  ctx->volume -= val;
  if (ctx->volume < 0) {
    ctx->volume = 0;
  }
}

static esp_err_t input_key_service_cb(periph_service_handle_t handle, periph_service_event_t* evt, void* ctx) {
  context_struct* cb_ctx = ctx;
  ESP_LOGI(TAG, "input_key_service_cb");
  if (evt->type == INPUT_KEY_SERVICE_ACTION_CLICK_RELEASE) {
    ESP_LOGI(TAG, "INPUT_KEY_SERVICE_ACTION_CLICK_RELEASE");
    int key_id = (int)evt->data;
    ESP_LOGI(TAG, "Input key id = %d", key_id);
    switch (key_id) {
      case INPUT_KEY_USER_ID_REC:
        cb_ctx->config->record_cb(cb_ctx->config->ctx);
        break;
      case INPUT_KEY_USER_ID_PLAY:
        cb_ctx->config->play_cb(cb_ctx->config->ctx);
        break;
      case INPUT_KEY_USER_ID_VOLUP:
        context_volup(cb_ctx, 10);
        cb_ctx->config->volume_set_cb(cb_ctx->volume, cb_ctx->config->ctx);
        break;
      case INPUT_KEY_USER_ID_VOLDOWN:
        context_voldown(cb_ctx, 10);
        cb_ctx->config->volume_set_cb(cb_ctx->volume, cb_ctx->config->ctx);
        break;
    }
  } else if (evt->type == INPUT_KEY_SERVICE_ACTION_PRESS) {
    ESP_LOGI(TAG, "INPUT_KEY_SERVICE_ACTION_PRESS");
  }

  return ESP_OK;
}
