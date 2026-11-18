/*
 * HVB2 wire protocol constants and on-wire formats.
 * Source of truth: HVB-wristband-ble-module-requirements.md §4.
 */
#pragma once

#include <stdint.h>

/* UUID base: 38dbdee0-XXXX-462f-85a6-42658cfe6238 */
#define HVB_UUID_SERVICE            0x0001
#define HVB_UUID_FILE_BLOCK         0x3000
#define HVB_UUID_FILE_LENGTH        0x3001
#define HVB_UUID_FILE_MAX_LENGTH    0x3002
#define HVB_UUID_FILE_CHECKSUM      0x3003
#define HVB_UUID_COMMAND            0x3004
#define HVB_UUID_TRANSFER_STATUS    0x3005
#define HVB_UUID_ERROR_MESSAGE      0x3006
#define HVB_UUID_SHOW_INFO          0x3007
#define HVB_UUID_START_SHOW         0x3008
#define HVB_UUID_STOP_SHOW          0x3009
#define HVB_UUID_COLOR              0x3010
#define HVB_UUID_FIRMWARE_VERSION   0x3011
#define HVB_UUID_STORAGE_STATUS     0x3012

#define HVB_ADV_NAME                "HVB"
#define HVB_GAP_NAME                "Human Video Board"
#define HVB_FIRMWARE_VERSION        2u

#define HVB_FILE_BLOCK_MAX          128u
#define HVB_ERROR_MESSAGE_MAX       128u
#define HVB_COLOR_LEN               6u      /* ASCII "RRGGBB" */
#define HVB_START_DELAY_MAX_MS      60000u

/* 3004 commands */
#define HVB_CMD_START_TRANSFER      1u
#define HVB_CMD_CANCEL_TRANSFER     2u

/* 3005 status values */
#define HVB_STATUS_SUCCESS          0u
#define HVB_STATUS_ERROR            1u
#define HVB_STATUS_IN_PROGRESS      2u

/* HVB2 binary show file (little-endian) */
#define HVB2_MAGIC                  "HVB2"
#define HVB2_HEADER_SIZE            16u
#define HVB2_FRAME_SIZE             5u

typedef struct __attribute__((packed)) {
    char     magic[4];
    uint32_t show_id;
    uint32_t total_frames;
    uint16_t fps;
    uint16_t frame_count;
} hvb2_header_t;

typedef struct __attribute__((packed)) {
    uint16_t when;
    uint8_t  r, g, b;
} hvb2_frame_t;



/* 3008 payload */
typedef struct __attribute__((packed)) {
    uint32_t show_id;
    uint32_t delay_ms;
} hvb_start_cmd_t;



/* 3012 payload */
typedef struct __attribute__((packed)) {
    uint32_t bytes_available;
    uint32_t bytes_used_by_shows;
    uint32_t show_count;
} hvb_storage_status_t;
