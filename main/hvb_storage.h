/*
 * Show store: multiple parsed HVB2 shows keyed by uint32 show id.
 * v1: RAM session storage (pending designer input D6).
 */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include "esp_err.h"
#include "hvb_protocol.h"

typedef struct {
    uint32_t            show_id;
    uint32_t            total_frames;
    uint16_t            fps;
    uint16_t            frame_count;
    const hvb2_frame_t *frames;
} hvb_show_t;

esp_err_t         hvb_storage_init(void);
esp_err_t         hvb_storage_store(const uint8_t *file, size_t len); /* validated HVB2 file */
const hvb_show_t *hvb_storage_find(uint32_t show_id);
void              hvb_storage_get_status(hvb_storage_status_t *out);  /* 3012 */
