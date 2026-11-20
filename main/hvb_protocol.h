/*
 * HVB2 wire protocol constants and on-wire formats.
 * Source of truth: HVB-wristband-ble-module-requirements.md §4.
 *
 * No stubs here: only constants and packed structs shared by hvb_ble,
 * hvb_transfer, hvb_storage and hvb_playback. Everything on the wire is
 * little-endian (which matches the ESP32-C3), so these structs can be
 * memcpy'd straight from/to BLE buffers.
 */
#pragma once

#include <stdint.h>

/* UUID base: 38b475a5-XXXX-5d96-fa0e-e9858cfe6238 */
/*  Builds the characteristic UUID but subs the 'XXXX' bytes with any of the protocol bytes below as needed 
    The macro bytes are REVERSED (little endian), because that is the expected format for NimBLE
*/

#define HVB_UUID128_BYTES(x)                \
        0x38, 0x62, 0xfe, 0x8c,             \
        0x85, 0xe9, 0x0e, 0xfa,             \
        0x96, 0x5d,                         \
        (x & 0xFF), ((x & 0xFF00) >> 8),    \
        0xa5, 0x75, 0xb4, 0x38


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

#define HVB_ADV_NAME_SIZE           8u
#define HVB_ADV_PREFIX              "HVB"
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
    char     magic[4];      /* "HVB2", not NUL-terminated */
    uint32_t show_id;       /* key the show is stored/started under */
    uint32_t total_frames;  /* show length in frames; playback ends here */
    uint16_t fps;           /* frames per second, converts ms -> frame */
    uint16_t frame_count;   /* number of hvb2_frame_t records that follow */
} hvb2_header_t;            /* 16 bytes, followed by frame_count frames */

typedef struct __attribute__((packed)) {
    uint16_t when;          /* frame number this color becomes active at */
    uint8_t  r, g, b;       /* color held until the next keyframe */
} hvb2_frame_t;             /* 5 bytes */



/* 3008 payload */
typedef struct __attribute__((packed)) {
    uint32_t show_id;       /* show to play; must already be stored, != 0 */
    uint32_t delay_ms;      /* start after this long, <= HVB_START_DELAY_MAX_MS */
} hvb_start_cmd_t;



/* 3012 payload */
typedef struct __attribute__((packed)) {
    uint32_t bytes_available;      /* room left for more shows */
    uint32_t bytes_used_by_shows;  /* memory taken by stored shows */
    uint32_t show_count;           /* number of shows stored */
} hvb_storage_status_t;
