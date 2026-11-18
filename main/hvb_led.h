/*
 * LED hardware abstraction (FR-8).
 * Pinout / topology / drive method pending designer inputs D1-D4.
 */
#pragma once

#include <stdint.h>
#include "esp_err.h"

esp_err_t hvb_led_init(void);
void      hvb_led_set_rgb(uint8_t r, uint8_t g, uint8_t b);
void      hvb_led_off(void);
