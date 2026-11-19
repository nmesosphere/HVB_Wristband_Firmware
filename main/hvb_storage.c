/*
 * Show store: holds parsed HVB2 shows, looked up by show id.
 *
 * v1 keeps shows in RAM for the session only (lost on reboot); flash
 * persistence is pending designer input D6.
 */
#include "hvb_storage.h"

// TODO: duplicate show id policy (replace vs reject) — FR-4

/*
 * hvb_storage_init — set up an empty show store.
 *
 * Called once from app_main. Should allocate/clear whatever container
 * holds the shows (array or list).
 *
 * Returns: ESP_OK on success; ESP_ERR_NO_MEM if allocation fails.
 * Stub:    returns ESP_OK.
 */
esp_err_t hvb_storage_init(void)
{
    // TODO
    return ESP_OK;
}

/*
 * hvb_storage_store — parse and keep a received HVB2 file.
 *
 * `file`/`len` is the complete, CRC-checked file from hvb_transfer. Should:
 *   - check len >= HVB2_HEADER_SIZE and magic == "HVB2";
 *   - check len == HVB2_HEADER_SIZE + HVB2_FRAME_SIZE * frame_count;
 *   - copy the data into memory storage owns (the transfer buffer is
 *     freed right after this call);
 *   - insert it keyed by the header's show_id, applying the duplicate-id
 *     policy above.
 *
 * Returns: ESP_OK if stored;
 *          ESP_ERR_INVALID_ARG for a NULL pointer or bad magic;
 *          ESP_ERR_INVALID_SIZE if the length doesn't match the header;
 *          ESP_ERR_NO_MEM if there's no room for the show.
 * Stub:    returns ESP_OK without storing anything.
 */
esp_err_t hvb_storage_store(const uint8_t *file, size_t len)
{
    (void)file;
    (void)len;
    // TODO: validate magic / length == 16 + 5*frame_count, parse, insert
    return ESP_OK;
}

/*
 * hvb_storage_find — look up a show.
 *
 * Returns: pointer to the stored show (header fields + frame array), or
 *          NULL if no show has that id. The memory is owned by storage and
 *          stays valid until that show is replaced/removed; playback keeps
 *          the pointer while playing.
 * Stub:    always returns NULL.
 */
const hvb_show_t *hvb_storage_find(uint32_t show_id)
{
    (void)show_id;
    // TODO
    return NULL;
}

/*
 * hvb_storage_get_status — fill in the 3012 STORAGE_STATUS value.
 *
 * Should write into *out: bytes still available for shows, bytes used by
 * stored shows, and the number of stored shows. No return value.
 * Stub:    leaves *out untouched.
 */
void hvb_storage_get_status(hvb_storage_status_t *out)
{
    (void)out;
    // TODO
}
