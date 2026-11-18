#include "hvb_ble.h"
#include "hvb_protocol.h"

// TODO: GATT service table (3000-3012) with access callbacks
// TODO: GAP event handler (connect -> HVB_EVT_CONNECTED, disconnect -> HVB_EVT_DISCONNECTED + re-advertise)

esp_err_t hvb_ble_init(void)
{
    // TODO: nimble_port_init, register GATT services, set device name, start host task
    return ESP_OK;
}

esp_err_t hvb_ble_start_advertising(void)
{
    // TODO: connectable adv with service UUID 0001 + name "HVB"
    return ESP_OK;
}

esp_err_t hvb_ble_disconnect(void)
{
    // TODO
    return ESP_OK;
}

void hvb_ble_notify_transfer_status(uint32_t status)
{
    (void)status;
    // TODO
}

void hvb_ble_notify_error_message(const char *msg)
{
    (void)msg;
    // TODO
}
