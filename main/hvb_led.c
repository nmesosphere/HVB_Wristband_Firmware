#include "hvb_led.h"

// TODO(D1-D4): LED topology, GPIOs, drive (LEDC PWM / RMT WS2812), polarity

esp_err_t hvb_led_init(void)
{
    // TODO
    return ESP_OK;
}

void hvb_led_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    (void)r;
    (void)g;
    (void)b;
    // TODO
}

void hvb_led_off(void)
{
    hvb_led_set_rgb(0, 0, 0);
}
