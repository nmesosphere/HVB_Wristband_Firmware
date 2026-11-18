/*
 * App task + runtime state machine.
 *
 * BLE callbacks never touch LEDs or do heavy work (§9.2). They post an
 * hvb_event_t to the app task, which owns all state transitions, LED
 * output, and the playback tick.
 */
#pragma once

#include <stdint.h>
#include "esp_err.h"

typedef enum {
    HVB_STATE_BOOT = 0,
    HVB_STATE_ADVERTISING,        /* no central connected */
    HVB_STATE_PAIRING,            /* connected, mirroring phone color sequence via 3010, awaiting user confirm */
    HVB_STATE_CONNECTED_IDLE,     /* confirmed; ready for uploads / commands */
    HVB_STATE_TRANSFERRING,       /* HVB2 upload in progress */
    HVB_STATE_ARMED,              /* start scheduled, waiting for start_ms */
    HVB_STATE_PLAYING,            /* local playback (phone link optional) */
} hvb_state_t;

typedef enum {
    HVB_EVT_CONNECTED,
    HVB_EVT_DISCONNECTED,
    HVB_EVT_COLOR,                /* 3010 write */
    HVB_EVT_TRANSFER_COMPLETE,    /* all bytes received; verify + store */
    HVB_EVT_TRANSFER_CANCEL,
    HVB_EVT_START_SHOW,           /* 3008 write */
    HVB_EVT_STOP_SHOW,            /* 3009 write */
    HVB_EVT_PAIRING_TIMEOUT,
} hvb_event_type_t;

typedef struct {
    hvb_event_type_t type;
    union {
        struct { uint8_t r, g, b; } color;
        struct { uint32_t show_id, delay_ms; } start;
    };
} hvb_event_t;

esp_err_t   hvb_app_init(void);
esp_err_t   hvb_app_post_event(const hvb_event_t *evt);   /* safe from BLE host context */
hvb_state_t hvb_app_get_state(void);
