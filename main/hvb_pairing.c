/*
 * Pairing confirmation (option A).
 *
 * After connect, the phone streams its seat-unique color sequence via
 * 3010 and the app task mirrors each color on the LEDs. The user compares
 * phone and band: if they match, the phone carries on with normal protocol
 * writes (which confirms pairing); if not, the phone disconnects. If the
 * phone goes quiet for too long, the band times out and disconnects.
 *
 * This module only tracks "are we pairing" and "when did we last hear from
 * the phone"; the LED colors themselves are set by the app task.
 */
#include "hvb_pairing.h"

// TODO: decide pairing timeout value and what counts as "confirmed"

/*
 * hvb_pairing_begin — start a pairing session. Called by the app task on
 * HVB_EVT_CONNECTED.
 *
 * Should: mark pairing active and record the current time (esp_timer) as
 * the last-activity timestamp. No return value.
 * Stub:    does nothing.
 */
void hvb_pairing_begin(void)
{
    // TODO
}

/*
 * hvb_pairing_on_color — note that a 3010 color write arrived.
 *
 * Should: if pairing is active, refresh the last-activity timestamp so the
 * timeout doesn't fire while the phone is still streaming colors.
 * No return value.
 * Stub:    does nothing.
 */
void hvb_pairing_on_color(void)
{
    // TODO
}

/*
 * hvb_pairing_confirm — the phone accepted this band.
 *
 * Called on the first non-3010 protocol write while pairing is active.
 * Should: mark pairing inactive (which stops the timeout); the app task
 * then moves to HVB_STATE_CONNECTED_IDLE. A no-op when not pairing.
 * No return value.
 * Stub:    does nothing.
 */
void hvb_pairing_confirm(void)
{
    // TODO
}

/*
 * hvb_pairing_abort — end pairing without confirmation.
 *
 * Called on disconnect or pairing timeout. Should: mark pairing inactive
 * and turn the LEDs off so the mirrored color doesn't stay lit.
 * Safe to call when not pairing. No return value.
 * Stub:    does nothing.
 */
void hvb_pairing_abort(void)
{
    // TODO: clear LEDs
}

/*
 * hvb_pairing_is_active — query pairing state.
 *
 * Returns: true between hvb_pairing_begin() and the next confirm/abort;
 *          false otherwise.
 * Stub:    always returns false.
 */
bool hvb_pairing_is_active(void)
{
    // TODO
    return false;
}

/*
 * hvb_pairing_tick — timeout check, called every app-task loop.
 *
 * now_ms is the current monotonic time in ms. Should: if pairing is active
 * and now_ms - last_activity exceeds the pairing timeout, post
 * HVB_EVT_PAIRING_TIMEOUT once (the app task then aborts and disconnects).
 * No return value.
 * Stub:    does nothing.
 */
void hvb_pairing_tick(uint32_t now_ms)
{
    (void)now_ms;
    // TODO
}
