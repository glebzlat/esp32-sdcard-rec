#include <input_key_service.h>

typedef void(*input_key_service_volume_set_cb_t)(int volume, void* ctx);
typedef void(*input_key_service_record_cb_t)(void* ctx);
typedef void(*input_key_service_play_cb_t)(void* ctx);

typedef struct {
  esp_periph_set_handle_t periph_set;
  input_key_service_volume_set_cb_t volume_set_cb;
  input_key_service_record_cb_t record_cb;
  input_key_service_play_cb_t play_cb;
  void* ctx;
} input_key_service_config;

periph_service_handle_t create_input_key_service(input_key_service_config* config);
void destroy_input_key_service(periph_service_handle_t input_service);
