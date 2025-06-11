#include <audio_common.h>
#include <audio_element.h>
#include <audio_pipeline.h>

audio_pipeline_handle_t setup_recording_pipeline(void);
void destroy_recording_pipeline(audio_pipeline_handle_t pipeline);
audio_pipeline_handle_t setup_playback_pipeline(void);
void destroy_playback_pipeline(audio_pipeline_handle_t pipeline);
