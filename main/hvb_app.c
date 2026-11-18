#include "hvb_app.h"

esp_err_t hvb_app_init(void)
{
    // TODO: create event queue
    // TODO: create app task (event handling + 1-5 ms playback tick)
    return ESP_OK;
}

esp_err_t hvb_app_post_event(const hvb_event_t *evt)
{
    (void)evt;
    // TODO: non-blocking enqueue
    return ESP_OK;
}

hvb_state_t hvb_app_get_state(void)
{
    // TODO
    return HVB_STATE_BOOT;
}
