/*
 * NimBLE GATT peripheral: HVB service 0001, characteristics 3000-3012,
 * advertising, connect/disconnect handling.
 *
 * What each function is supposed to do and return (and what the current
 * stub does instead) is documented above each function in hvb_ble.c.
 */
#pragma once

#include <stdint.h>
#include "esp_err.h"

esp_err_t hvb_ble_init(void);
esp_err_t hvb_ble_start_advertising(void);
esp_err_t hvb_ble_disconnect(void);

/* Push updated values to readable/notify characteristics */
void hvb_ble_notify_transfer_status(uint32_t status);
void hvb_ble_notify_error_message(const char *msg);
