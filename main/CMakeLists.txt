idf_component_register(
    SRCS
        "main.c"
        "hvb_app.c"
        "hvb_ble.c"
        "hvb_pairing.c"
        "hvb_transfer.c"
        "hvb_storage.c"
        "hvb_playback.c"
        "hvb_led.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES bt nvs_flash esp_timer
)
