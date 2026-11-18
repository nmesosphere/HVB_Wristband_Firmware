/*
 * HVB wristband firmware — entry point.
 *
 * Target: ESP32-C3-MINI-1, ESP-IDF + NimBLE. No Arduino APIs.
 */
#include "hvb_app.h"

void app_main(void)
{
    // TODO: init NVS (required by NimBLE)
    // TODO: hvb_led_init()
    // TODO: hvb_storage_init()
    // TODO: hvb_app_init()  (event queue + app task)
    // TODO: hvb_ble_init()  (GATT service, start advertising)
}
