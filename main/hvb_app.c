/*
 * App task + runtime state machine.
 *
 * This is the only task that changes hvb_state_t, drives the LEDs and runs
 * the playback/pairing timers. Everything else (mainly the NimBLE host
 * task) talks to it by posting hvb_event_t via hvb_app_post_event().
 *
 * Intended event handling (to be implemented in the task loop):
 *
 *   HVB_EVT_CONNECTED          -> hvb_pairing_begin(); state = PAIRING
 *   HVB_EVT_COLOR              -> hvb_led_set_rgb(r, g, b);
 *                                 hvb_pairing_on_color() if pairing
 *   HVB_EVT_TRANSFER_COMPLETE  -> hvb_transfer_finalize() (CRC + store);
 *                                 state back to CONNECTED_IDLE
 *   HVB_EVT_TRANSFER_CANCEL    -> hvb_transfer_cancel()
 *   HVB_EVT_START_SHOW         -> hvb_playback_start(show_id, delay_ms);
 *                                 state = ARMED on success
 *   HVB_EVT_STOP_SHOW          -> hvb_playback_stop(); state = CONNECTED_IDLE
 *   HVB_EVT_PAIRING_TIMEOUT    -> hvb_pairing_abort(); hvb_ble_disconnect()
 *   HVB_EVT_DISCONNECTED       -> hvb_pairing_abort(); hvb_transfer_cancel();
 *                                 a show that is ARMED/PLAYING keeps running
 *                                 (phone link is optional during playback);
 *                                 otherwise state = ADVERTISING
 *
 * Every loop iteration (every 1-5 ms, via the queue-receive timeout) it
 * also calls hvb_pairing_tick(now_ms) and hvb_playback_update(now_ms), and
 * moves ARMED -> PLAYING -> idle as playback reports.
 */
#include "hvb_app.h"

/*
 * hvb_app_init — create the event queue and the app task.
 *
 * Should: create a FreeRTOS queue of hvb_event_t (copied by value) and
 * spawn the app task described above. Must be called before hvb_ble_init().
 *
 * Returns: ESP_OK on success; ESP_ERR_NO_MEM if the queue or task could
 *          not be created.
 * Stub:    returns ESP_OK without creating anything.
 */
esp_err_t hvb_app_init(void)
{
    // TODO: create event queue
    // TODO: create app task (event handling + 1-5 ms playback tick)
    return ESP_OK;
}

/*
 * hvb_app_post_event — hand an event to the app task.
 *
 * Called from BLE access/GAP callbacks (NimBLE host task) and from other
 * modules. Copies *evt into the queue with a zero timeout: it must NEVER
 * block, because blocking the NimBLE host stalls the whole BLE stack.
 *
 * Returns: ESP_OK if queued;
 *          ESP_ERR_INVALID_ARG if evt is NULL;
 *          ESP_ERR_INVALID_STATE if called before hvb_app_init();
 *          ESP_ERR_TIMEOUT if the queue is full and the event was dropped.
 * Stub:    discards the event and returns ESP_OK.
 */
esp_err_t hvb_app_post_event(const hvb_event_t *evt)
{
    (void)evt;
    // TODO: non-blocking enqueue
    return ESP_OK;
}

/*
 * hvb_app_get_state — read the current runtime state.
 *
 * Read-only view for other modules (e.g. BLE deciding whether a write is
 * allowed right now). Only the app task ever writes the state.
 *
 * Returns: the current hvb_state_t.
 * Stub:    always returns HVB_STATE_BOOT.
 */
hvb_state_t hvb_app_get_state(void)
{
    // TODO
    return HVB_STATE_BOOT;
}
