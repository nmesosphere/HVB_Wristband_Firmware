/*
 * LED hardware abstraction (FR-8).
 *
 * The rest of the firmware only thinks in 8-bit RGB; this file maps that
 * onto whatever the real hardware turns out to be. Only the app task
 * calls these functions, so no locking is needed.
 */
#include "hvb_led.h"

// TODO(D1-D4): LED topology, GPIOs, drive (LEDC PWM / RMT WS2812), polarity

/*
 * hvb_led_init — configure the LED hardware and turn it off.
 *
 * Should: set up the GPIOs and the chosen driver (LEDC PWM channels for a
 * discrete RGB LED, or RMT for WS2812-style LEDs), then leave the LEDs
 * off. Called once from app_main before anything else uses the LEDs.
 *
 * Returns: ESP_OK on success, or the error from the GPIO/LEDC/RMT driver.
 * Stub:    returns ESP_OK without touching hardware.
 */
esp_err_t hvb_led_init(void)
{
    // TODO
    return ESP_OK;
}

/*
 * hvb_led_set_rgb — show a color.
 *
 * r, g, b are 0-255 (0 = off, 255 = full). Should apply whatever the
 * hardware needs (inverted polarity for common-anode, channel order,
 * brightness/gamma correction) and update the output immediately. Must be
 * fast: it's called from the playback tick every few ms. No return value.
 * Stub:    does nothing.
 */
void hvb_led_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    (void)r;
    (void)g;
    (void)b;
    // TODO
}

/*
 * hvb_led_off — turn all LEDs off. Already implemented as set_rgb(0,0,0),
 * so it starts working as soon as hvb_led_set_rgb() does.
 */
void hvb_led_off(void)
{
    hvb_led_set_rgb(0, 0, 0);
}
