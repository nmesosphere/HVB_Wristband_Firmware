# HVB Wristband Firmware

ESP-IDF + NimBLE firmware for an ESP32-C3-MINI-1 wristband that acts as an HVB2 BLE peripheral for the Human Video Board phone app. Spec: [HVB-wristband-ble-module-requirements.md](HVB-wristband-ble-module-requirements.md).

**Status:** scaffold only. Every function is a stub.

## Layout

| File | Responsibility |
|------|----------------|
| `main/main.c` | `app_main`: init order |
| `main/hvb_app.*` | App task, event queue, runtime state machine |
| `main/hvb_ble.*` | GATT service `0001`, chars `3000`–`3012`, advertising |
| `main/hvb_pairing.*` | Post-connect color-sequence mirroring + confirm/timeout |
| `main/hvb_protocol.h` | UUIDs, constants, HVB2 wire structs |
| `main/hvb_transfer.*` | Upload state machine + CRC32 |
| `main/hvb_storage.*` | Show store keyed by show id |
| `main/hvb_playback.*` | Delayed start, local playback, stop |
| `main/hvb_led.*` | LED driver (`hvb_led_set_rgb`) |

## Build

```
idf.py set-target esp32c3
idf.py build
idf.py -p <PORT> flash monitor
```

## Open inputs

- Designer inputs D1–D5 (LED topology, GPIOs, drive method, board, toolchain version)
- Storage policy (RAM vs flash), min show capacity
- Pairing timeout and what counts as "confirmed"
- Duplicate show id policy
