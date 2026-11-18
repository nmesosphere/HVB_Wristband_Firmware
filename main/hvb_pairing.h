/*
 * Pairing confirmation (option A): after the phone connects, it streams
 * its seat color sequence via 3010 and the band mirrors it. The user
 * confirms visually; on mismatch the phone disconnects and the band
 * returns to advertising.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

void hvb_pairing_begin(void);           /* on connect */
void hvb_pairing_on_color(void);        /* any 3010 write refreshes activity */
void hvb_pairing_confirm(void);         /* first non-color protocol write = phone accepted us */
void hvb_pairing_abort(void);           /* disconnect or timeout */
bool hvb_pairing_is_active(void);
void hvb_pairing_tick(uint32_t now_ms); /* timeout check */
