/*
 * Local show playback (FR-5 / FR-6).
 *
 * A show is a list of keyframes (hvb2_frame_t). A keyframe's `when` is the
 * frame number at which its color becomes active; it stays active until
 * the next keyframe. The current frame number comes from elapsed time and
 * the show's fps. Playback runs entirely on the band, so it keeps going if
 * the phone disconnects. Driven only from the app task.
 */
#include "hvb_playback.h"

/*
 * hvb_playback_start — arm a show to start after a delay.
 *
 * Should:
 *   - reject show_id == 0 or delay_ms > HVB_START_DELAY_MAX_MS (60 s);
 *   - look the show up with hvb_storage_find(); fail if it isn't stored;
 *   - replace any show that is already armed or playing;
 *   - record the show and start_ms = now + delay_ms; state = armed.
 * The LEDs don't change until hvb_playback_update() reaches start_ms.
 *
 * Returns: ESP_OK if armed;
 *          ESP_ERR_INVALID_ARG for show_id 0 or a delay over 60 s;
 *          ESP_ERR_NOT_FOUND if no show with that id is stored.
 * Stub:    returns ESP_OK; nothing is armed.
 */
esp_err_t hvb_playback_start(uint32_t show_id, uint32_t delay_ms)
{
    (void)show_id;
    (void)delay_ms;
    // TODO: reject show_id == 0 / delay_ms > 60000; start_ms = now + delay_ms
    return ESP_OK;
}

/*
 * hvb_playback_stop — cancel any armed or playing show.
 *
 * Should: clear the armed/playing state and turn the LEDs off. Safe to
 * call when nothing is playing. No return value.
 * Stub:    does nothing.
 */
void hvb_playback_stop(void)
{
    // TODO
}

/*
 * hvb_playback_update — advance playback; call every 1-5 ms.
 *
 * now_ms is the current monotonic time in ms. Should:
 *   - if armed and now_ms >= start_ms: switch to playing;
 *   - if playing: frame = (now_ms - start_ms) * fps / 1000 (use 64-bit
 *     math to avoid overflow); if frame >= total_frames, stop (LEDs off);
 *     otherwise find the last keyframe with when <= frame and, if its
 *     color differs from what's showing, call hvb_led_set_rgb().
 * Does nothing when idle. No return value.
 * Stub:    does nothing.
 */
void hvb_playback_update(uint32_t now_ms)
{
    (void)now_ms;
    // TODO: frame = elapsed_ms * fps / 1000; apply active keyframe; stop at total_frames
}

/*
 * hvb_playback_is_armed
 * Returns: true if a show is scheduled but start_ms hasn't been reached.
 * Stub:    always returns false.
 */
bool hvb_playback_is_armed(void)
{
    // TODO
    return false;
}

/*
 * hvb_playback_is_playing
 * Returns: true while a show is running (from start_ms to its last frame).
 * Stub:    always returns false.
 */
bool hvb_playback_is_playing(void)
{
    // TODO
    return false;
}
