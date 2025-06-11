#include <fatfs_stream.h>
#include <i2s_stream.h>
#include <wav_encoder.h>
#include <wav_decoder.h>
#include <esp_log.h>

#include "pipeline.h"

static char const* TAG = "PIPELINE";

audio_pipeline_handle_t setup_recording_pipeline(void) {
  audio_pipeline_handle_t pipeline;
  audio_element_handle_t fatfs_writer, i2s_reader, audio_encoder;
  int channel_format = I2S_CHANNEL_TYPE_RIGHT_LEFT;
  int sample_rate = 16000;

  ESP_LOGI(TAG, "Create recording pipeline");
  audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
  pipeline = audio_pipeline_init(&pipeline_cfg);
  mem_assert(pipeline);

  ESP_LOGI(TAG, "Create fatfs stream");
  fatfs_stream_cfg_t fatfs_cfg = FATFS_STREAM_CFG_DEFAULT();
  fatfs_cfg.type = AUDIO_STREAM_WRITER;
  fatfs_writer = fatfs_stream_init(&fatfs_cfg);

  ESP_LOGI(TAG, "Create i2s stream");
  i2s_stream_cfg_t i2s_cfg = I2S_STREAM_CFG_DEFAULT();
  i2s_cfg.type = AUDIO_STREAM_READER;
  i2s_cfg.chan_cfg.id = 1;
  i2s_stream_set_channel_type(&i2s_cfg, channel_format);
  i2s_cfg.std_cfg.clk_cfg.sample_rate_hz = sample_rate;
  i2s_reader = i2s_stream_init(&i2s_cfg);

  ESP_LOGI(TAG, "Create audio encoder");
  wav_encoder_cfg_t wav_cfg = DEFAULT_WAV_ENCODER_CONFIG();
  audio_encoder = wav_encoder_init(&wav_cfg);

  ESP_LOGI(TAG, "Register elements to the pipeline");
  audio_pipeline_register(pipeline, i2s_reader, "i2s");
  audio_pipeline_register(pipeline, audio_encoder, "wav");
  audio_pipeline_register(pipeline, fatfs_writer, "file");

  ESP_LOGI(TAG, "Link audio elements: [codec] -> i2s_reader -> audio_encoder -> fatfs_writer -> [sdcard]");
  char const* link_tag[] = {"i2s", "wav", "file"};
  audio_pipeline_link(pipeline, &link_tag[0], 3);

  ESP_LOGI(TAG, "Set music info to fatfs stream");
  audio_element_info_t music_info = {0};
  audio_element_getinfo(i2s_reader, &music_info);
  ESP_LOGI(TAG, "Save the recording info to the fatfs stream: sample_rates=%d, bits=%d, channels=%d",
           music_info.sample_rates, music_info.bits, music_info.channels);
  audio_element_setinfo(fatfs_writer, &music_info);

  ESP_LOGI(TAG, "Set up uri");
  audio_element_set_uri(fatfs_writer, "/sdcard/rec.wav");

  audio_pipeline_run(pipeline);

  return pipeline;
}

void destroy_recording_pipeline(audio_pipeline_handle_t pipeline) {
  audio_pipeline_stop(pipeline);
  audio_pipeline_wait_for_stop(pipeline);
  audio_pipeline_terminate(pipeline);

  audio_element_handle_t fatfs, wav, i2s;
  fatfs = audio_pipeline_get_el_by_tag(pipeline, "file");
  wav = audio_pipeline_get_el_by_tag(pipeline, "wav");
  i2s = audio_pipeline_get_el_by_tag(pipeline, "i2s");

  audio_pipeline_unregister(pipeline, fatfs);
  audio_pipeline_unregister(pipeline, wav);
  audio_pipeline_unregister(pipeline, i2s);

  audio_pipeline_deinit(pipeline);

  audio_element_deinit(fatfs);
  audio_element_deinit(wav);
  audio_element_deinit(i2s);
}

audio_pipeline_handle_t setup_playback_pipeline(void) {
  audio_pipeline_handle_t pipeline;
  audio_element_handle_t fatfs_reader, i2s_writer, audio_decoder;

  ESP_LOGI(TAG, "Create playback pipeline");
  audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
  pipeline = audio_pipeline_init(&pipeline_cfg);
  mem_assert(pipeline);

  ESP_LOGI(TAG, "Create fatfs reader stream");
  fatfs_stream_cfg_t fatfs_cfg = FATFS_STREAM_CFG_DEFAULT();
  fatfs_cfg.type = AUDIO_STREAM_READER;
  fatfs_reader = fatfs_stream_init(&fatfs_cfg);

  ESP_LOGI(TAG, "Create i2s writer stream");
  i2s_stream_cfg_t i2s_cfg = I2S_STREAM_CFG_DEFAULT();
  i2s_cfg.type = AUDIO_STREAM_WRITER;
  i2s_writer = i2s_stream_init(&i2s_cfg);

  ESP_LOGI(TAG, "Create audio decoder");
  wav_decoder_cfg_t wav_cfg = DEFAULT_WAV_DECODER_CONFIG();
  audio_decoder = wav_decoder_init(&wav_cfg);

  ESP_LOGI(TAG, "Register elements to the pipeline");
  audio_pipeline_register(pipeline, fatfs_reader, "file");
  audio_pipeline_register(pipeline, audio_decoder, "wav");
  audio_pipeline_register(pipeline, i2s_writer, "i2s");

  ESP_LOGI(TAG, "Link audio elements: [sdcard] -> fatfs_reader -> audio_decoder -> i2s_writer -> [codec]");
  char const* link_tag[] = {"file", "wav", "i2s"};
  audio_pipeline_link(pipeline, &link_tag[0], 3);

  audio_element_set_uri(fatfs_reader, "/sdcard/rec.wav");

  audio_pipeline_run(pipeline);

  return pipeline;
}

void destroy_playback_pipeline(audio_pipeline_handle_t pipeline) {
  audio_pipeline_stop(pipeline);
  audio_pipeline_wait_for_stop(pipeline);
  audio_pipeline_terminate(pipeline);

  audio_element_handle_t fatfs, wav, i2s;
  fatfs = audio_pipeline_get_el_by_tag(pipeline, "file");
  wav = audio_pipeline_get_el_by_tag(pipeline, "wav");
  i2s = audio_pipeline_get_el_by_tag(pipeline, "i2s");

  audio_pipeline_unregister(pipeline, fatfs);
  audio_pipeline_unregister(pipeline, wav);
  audio_pipeline_unregister(pipeline, i2s);

  audio_element_deinit(fatfs);
  audio_element_deinit(wav);
  audio_element_deinit(i2s);

  audio_pipeline_deinit(pipeline);
}
