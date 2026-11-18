#include "hvb_playback.h"

esp_err_t hvb_playback_start(uint32_t show_id, uint32_t delay_ms)
{
    (void)show_id;
    (void)delay_ms;
    // TODO: reject show_id == 0 / delay_ms > 60000; start_ms = now + delay_ms
    return ESP_OK;
}

void hvb_playback_stop(void)
{
    // TODO
}

void hvb_playback_update(uint32_t now_ms)
{
    (void)now_ms;
    // TODO: frame = elapsed_ms * fps / 1000; apply active keyframe; stop at total_frames
}

bool hvb_playback_is_armed(void)
{
    // TODO
    return false;
}

bool hvb_playback_is_playing(void)
{
    // TODO
    return false;
}
