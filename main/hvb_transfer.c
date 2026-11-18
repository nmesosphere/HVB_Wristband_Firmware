#include "hvb_transfer.h"
#include "hvb_protocol.h"

void hvb_transfer_set_show_id(uint32_t show_id)
{
    (void)show_id;
    // TODO
}

void hvb_transfer_set_length(uint32_t length)
{
    (void)length;
    // TODO
}

void hvb_transfer_set_checksum(uint32_t crc)
{
    (void)crc;
    // TODO
}

esp_err_t hvb_transfer_command(uint32_t cmd)
{
    (void)cmd;
    // TODO: 1 = allocate buffer + IN_PROGRESS, 2 = cancel
    return ESP_OK;
}

esp_err_t hvb_transfer_write_block(const uint8_t *data, size_t len)
{
    (void)data;
    (void)len;
    // TODO: append; when length reached post HVB_EVT_TRANSFER_COMPLETE
    return ESP_OK;
}

esp_err_t hvb_transfer_finalize(void)
{
    // TODO: CRC32 check, header validation, hvb_storage_store()
    return ESP_OK;
}

void hvb_transfer_cancel(void)
{
    // TODO: free buffer, status = ERROR
}

uint32_t hvb_transfer_max_length(void)
{
    // TODO: derive from free heap minus BLE reserve (§9.5)
    return 0;
}

uint32_t hvb_transfer_status(void)
{
    // TODO
    return HVB_STATUS_SUCCESS;
}

hvb_xfer_state_t hvb_transfer_state(void)
{
    // TODO
    return HVB_XFER_IDLE;
}

uint32_t hvb_crc32(const uint8_t *data, size_t len)
{
    (void)data;
    (void)len;
    // TODO: port reference crc32 / crc32ForByte exactly
    return 0;
}
