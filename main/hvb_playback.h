/*
 * Local show playback (FR-5 / FR-6). Driven from the app task tick.
 *
 * What each function is supposed to do and return (and what the current
 * stub does instead) is documented above each function in hvb_playback.c.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

esp_err_t hvb_playback_start(uint32_t show_id, uint32_t delay_ms); /* re-arm replaces prior */
void      hvb_playback_stop(void);                                 /* stop + clear LEDs */
void      hvb_playback_update(uint32_t now_ms);                    /* call every 1-5 ms */
bool      hvb_playback_is_armed(void);
bool      hvb_playback_is_playing(void);
