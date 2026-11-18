#include "hvb_storage.h"

// TODO: duplicate show id policy (replace vs reject) — FR-4

esp_err_t hvb_storage_init(void)
{
    // TODO
    return ESP_OK;
}

esp_err_t hvb_storage_store(const uint8_t *file, size_t len)
{
    (void)file;
    (void)len;
    // TODO: validate magic / length == 16 + 5*frame_count, parse, insert
    return ESP_OK;
}

const hvb_show_t *hvb_storage_find(uint32_t show_id)
{
    (void)show_id;
    // TODO
    return NULL;
}

void hvb_storage_get_status(hvb_storage_status_t *out)
{
    (void)out;
    // TODO
}
