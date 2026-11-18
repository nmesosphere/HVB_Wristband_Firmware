/*
 * HVB2 file transfer state machine (§4.2) + CRC32 (§4.3).
 */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include "esp_err.h"

typedef enum {
    HVB_XFER_IDLE = 0,
    HVB_XFER_IN_PROGRESS,
    HVB_XFER_VERIFYING,
} hvb_xfer_state_t;

void      hvb_transfer_set_show_id(uint32_t show_id);    /* 3007 */
void      hvb_transfer_set_length(uint32_t length);      /* 3001 */
void      hvb_transfer_set_checksum(uint32_t crc);       /* 3003 */
esp_err_t hvb_transfer_command(uint32_t cmd);            /* 3004 */
esp_err_t hvb_transfer_write_block(const uint8_t *data, size_t len); /* 3000 */
esp_err_t hvb_transfer_finalize(void);                   /* CRC + validate + store (app task) */
void      hvb_transfer_cancel(void);

uint32_t         hvb_transfer_max_length(void);          /* 3002 */
uint32_t         hvb_transfer_status(void);              /* 3005 */
hvb_xfer_state_t hvb_transfer_state(void);

/* zlib-compatible CRC32, ported from reference hvb_firmware */
uint32_t hvb_crc32(const uint8_t *data, size_t len);
