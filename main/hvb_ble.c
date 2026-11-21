/*
 * NimBLE GATT peripheral for the HVB service.
 *
 * Runs in the NimBLE host task. Access callbacks must stay short: they
 * parse/validate the write, then either call a cheap setter
 * (hvb_transfer_*) or post an hvb_event_t to the app task. They never
 * touch LEDs or CRC/parse whole files.
 *
 * Intended characteristic behaviour (service 0001, UUIDs in hvb_protocol.h):
 *
 *   3000 FILE_BLOCK        write  <= 128 bytes  -> hvb_transfer_write_block()
 *   3001 FILE_LENGTH       write  uint32        -> hvb_transfer_set_length()
 *   3002 FILE_MAX_LENGTH   read   uint32        <- hvb_transfer_max_length()
 *   3003 FILE_CHECKSUM     write  uint32 CRC32  -> hvb_transfer_set_checksum()
 *   3004 COMMAND           write  uint32        -> hvb_transfer_command()
 *                                                  (1 = start, 2 = cancel)
 *   3005 TRANSFER_STATUS   read/notify uint32   <- hvb_transfer_status()
 *   3006 ERROR_MESSAGE     read/notify text     (last error, <= 128 bytes)
 *   3007 SHOW_INFO         write  uint32        -> hvb_transfer_set_show_id()
 *   3008 START_SHOW        write  hvb_start_cmd_t -> post HVB_EVT_START_SHOW
 *   3009 STOP_SHOW         write                -> post HVB_EVT_STOP_SHOW
 *   3010 COLOR             write  "RRGGBB"      -> parse hex, post HVB_EVT_COLOR
 *   3011 FIRMWARE_VERSION  read   uint32        <- HVB_FIRMWARE_VERSION
 *   3012 STORAGE_STATUS    read   hvb_storage_status_t <- hvb_storage_get_status()
 *
 * Writes with the wrong length or malformed content should be rejected
 * with an ATT error (e.g. BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN) rather than
 * silently ignored. While pairing is active, the first write to anything
 * other than 3010 counts as the phone confirming us (hvb_pairing_confirm).
 *
 * GAP events: connect    -> post HVB_EVT_CONNECTED;
 *             disconnect -> post HVB_EVT_DISCONNECTED and re-advertise.
 */
#include <stdio.h>
#include <string.h>
#include "hvb_ble.h"
#include "hvb_protocol.h"

#include "esp_log.h"
#include "esp_mac.h"    // For building the wristband's ID
#include "nimble/nimble_port_freertos.h"
#include "nimble/nimble_port.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "host/ble_hs.h"
#include "host/util/util.h"


static const char *TAG = "hvb_ble";
static const ble_uuid128_t hvb_svc_uuid = BLE_UUID128_INIT(HVB_UUID128_BYTES(HVB_UUID_SERVICE));

static char wristband_id[HVB_ADV_NAME_SIZE + 1];  // '+ 1' to account for the '\0' terminator
static uint8_t hvb_own_addr_type;
static int hvb_gap_event(struct ble_gap_event *event, void *arg);

/* Build the unique ID for the wristband based on its MAC address (for testing, debugging, and fallback BLE connections)*/

static char *create_wristband_ID(void){
    
    uint8_t mac_address_byte[6] = {0};
    esp_err_t err = esp_read_mac(mac_address_byte, ESP_MAC_BT);

    if (err != ESP_OK){
        ESP_LOGW(TAG, "esp_read_mac(BT) failed (%s); device name suffix will now default to 0000",
            esp_err_to_name(err));
    }

    /* Fills the wristband_id buffer in this format: "prefix-XXXX" where 'XX' is a specific byte from the MAC address
        prefix -> "HVB"
        Use last 2 bytes from the MAC address -> XXXX = <byte_4><byte_5>
    */
    snprintf(wristband_id, sizeof(wristband_id), "%s-%02X%02X", 
            HVB_ADV_PREFIX, mac_address_byte[4], mac_address_byte[5]);

    ESP_LOGI(TAG, "Wristband ID: %s", wristband_id);  
    return wristband_id;
}



/* RTOS tasks */
static void hvb_on_stack_reset(int reason){
    ESP_LOGW(TAG, "NimBLE stack was reset for reason: %d", reason);
}

static void hvb_on_stack_sync(void) {
    
    /* 'rc' = 'Return Code' */
    int rc = ble_hs_util_ensure_addr(0);

    if(rc != 0){
        ESP_LOGE(TAG, "Could not configure device's BLE address rc = %d", rc);
        return;
    }
    
    rc = ble_hs_id_infer_auto(0, &hvb_own_addr_type);

    if(rc != 0){
        ESP_LOGE(TAG, "Could not choose device's BLE address type: rc = %d", rc);
        return;
    }


    
    /* Stack is ready: the only safe point to start advertising */
    
    ESP_LOGI(TAG, "host synced, name=\"%s\"", ble_svc_gap_device_name());
    
    /* No automatic retry yet: if this fails the band stays silent until reset */
    if (hvb_ble_start_advertising() != ESP_OK){
        ESP_LOGE(TAG, "Wristband cannot advertise");
    }
}

static void hvb_host_task(void *param){
    ESP_LOGI(TAG, "host task has started");
    nimble_port_run();
    nimble_port_freertos_deinit();
}

static void hvb_host_config_init(void) {
    /* Set host callbacks */
    ble_hs_cfg.reset_cb = hvb_on_stack_reset;
    ble_hs_cfg.sync_cb = hvb_on_stack_sync;
    /* Not needed yet: gatts_register_cb (debug logging of registered
     * attributes) and store_status_cb / ble_store_config_init (bonding). */
}

// TODO: GATT service table (3000-3012) with access callbacks
// TODO: GAP event handler (connect -> HVB_EVT_CONNECTED, disconnect -> HVB_EVT_DISCONNECTED + re-advertise)

/*
 * hvb_ble_init — bring up the NimBLE stack and the HVB GATT service.
 *
 * Should: nimble_port_init(), init the GAP/GATT services, register the
 * table above, set the GAP device name to HVB_GAP_NAME, set a host sync
 * callback that calls hvb_ble_start_advertising(), then start the host
 * task with nimble_port_freertos_init().
 *
 * Returns: ESP_OK on success, or the error from the failing NimBLE call.
 * Stub:    returns ESP_OK; BLE is not started.
 */
esp_err_t hvb_ble_init(void)
{
    // DONE: nimble_port_init, built-in GAP/GATT services, device name, start host task
    // TODO: (item 10): register the HVB GATT table (ble_gatts_count_cfg + ble_gatts_add_svcs)

    /* Create the ID for this wristband */
    create_wristband_ID();

    /* nimble_port_init */
    esp_err_t ret = nimble_port_init();         // Starts the NimBLE host stack (handles higher-level BT functions)
    if (ret != ESP_OK){
        ESP_LOGE(TAG, "failed to initialize nimble stack: %s", esp_err_to_name(ret));
        return ret;
    }

    /* NimBLE's built-in GAP + GATT services, and the GAP device name */
    ble_svc_gap_init();
    int rc = ble_svc_gap_device_name_set(HVB_GAP_NAME);
    if (rc != 0) {
        ESP_LOGE(TAG, "failed to set device name, rc=%d", rc);
        nimble_port_deinit();
        return ESP_FAIL;
    }
    ble_svc_gatt_init();


    /* TODO: (item 10): register HVB service table here */


    /* Host configuration */
    hvb_host_config_init();

    /* Start host task*/
    nimble_port_freertos_init(hvb_host_task);


    return ret;
}

/*
 * hvb_ble_start_advertising — make the band discoverable/connectable.
 *
 * Should: start undirected connectable advertising whose payload contains
 * the 128-bit service UUID 0001 and the name wristband_id ("HVB-XXXX").
 * Called on host sync and after every disconnect.
 *
 * Returns: ESP_OK if advertising started;
 *          ESP_FAIL if setting the fields or starting advertising fails.
 */
esp_err_t hvb_ble_start_advertising(void)
{
    // DONE: connectable adv with service UUID 0001 + name "HVB-XXXX"
    struct ble_hs_adv_fields fields;

    /** From ESP-IDF Documentation
     *  Set the advertisement data included in our advertisements:
     *     o Flags (indicates advertisement type and other general info).
     *     o Device name (wristband_id, "HVB-XXXX").
     *     o 128-bit HVB service UUID.
     *  Total = 3 + 10 + 18 = 31 bytes, exactly the legacy advertising limit.
     */

    memset(&fields, 0, sizeof(fields));      // First, zero-out the fields, so it's not filled with garbage values. Also, zeroes-out unused fields

    // Set the advertisement fields to actual values
    fields.flags = BLE_HS_ADV_F_DISC_GEN |
                   BLE_HS_ADV_F_BREDR_UNSUP;
    
    fields.name = (const uint8_t *)wristband_id;    // NimBLE expects bytes (uint8_t), our buffer is char
    fields.name_len = strlen(wristband_id);
    fields.name_is_complete = 1;

    fields.uuids128 = &hvb_svc_uuid;
    fields.num_uuids128 = 1;
    fields.uuids128_is_complete = 1;

    int rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0){
        ESP_LOGE(TAG, "Failed to set BLE advertising fields, rc=%d", rc);
        return ESP_FAIL;
    }

    struct ble_gap_adv_params adv_params;
    memset(&adv_params, 0, sizeof(adv_params));

    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    rc = ble_gap_adv_start(hvb_own_addr_type, NULL, BLE_HS_FOREVER, &adv_params, hvb_gap_event, NULL);

    if (rc != 0){
        ESP_LOGE(TAG, "Failed to start BLE advertising, rc=%d", rc);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Advertising as \"%s\"", wristband_id);
    return ESP_OK;
}

/*
 * hvb_gap_event — NimBLE calls this for connection/advertising events.
 *
 * Minimal version for item 3: log connect/disconnect and keep the band
 * discoverable. Item 7 adds: storing conn_handle, posting
 * HVB_EVT_CONNECTED / HVB_EVT_DISCONNECTED, subscribe tracking.
 *
 * Returns: 0 (NimBLE ignores the value for these events).
 */
static int hvb_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type) {
    case BLE_GAP_EVENT_CONNECT:
        ESP_LOGI(TAG, "connect %s, status=%d",
                 event->connect.status == 0 ? "ok" : "failed", event->connect.status);
        if (event->connect.status != 0) {
            hvb_ble_start_advertising();        // attempt failed: advertise again
        }
        break;

    case BLE_GAP_EVENT_DISCONNECT:
        ESP_LOGI(TAG, "disconnect, reason=%d", event->disconnect.reason);
        hvb_ble_start_advertising();            // become discoverable again
        break;

    case BLE_GAP_EVENT_ADV_COMPLETE:
        ESP_LOGI(TAG, "advertising complete, reason=%d", event->adv_complete.reason);
        hvb_ble_start_advertising();
        break;

    default:
        break;
    }
    return 0;
}

/*
 * hvb_ble_disconnect — drop the current central.
 *
 * Used by the app task when pairing times out. Should call
 * ble_gap_terminate() on the stored connection handle; the normal GAP
 * disconnect event then fires and re-advertising happens there.
 *
 * Returns: ESP_OK if termination was requested;
 *          ESP_ERR_INVALID_STATE if nothing is connected.
 * Stub:    returns ESP_OK; does nothing.
 */
esp_err_t hvb_ble_disconnect(void)
{
    // TODO:
    return ESP_OK;
}

/*
 * hvb_ble_notify_transfer_status — publish a new 3005 value.
 *
 * Should: store `status` (HVB_STATUS_SUCCESS / ERROR / IN_PROGRESS) as the
 * readable value of 3005 and send a notification if the phone has
 * subscribed. No return value; a failed notify is just logged.
 * Stub:    does nothing.
 */
void hvb_ble_notify_transfer_status(uint32_t status)
{
    (void)status;
    // TODO:
}

/*
 * hvb_ble_notify_error_message — publish a new 3006 value.
 *
 * Should: copy `msg` (truncated to HVB_ERROR_MESSAGE_MAX bytes) into the
 * 3006 value and notify the phone if subscribed. Used to explain why a
 * transfer or command failed (bad CRC, out of memory, unknown show, ...).
 * No return value.
 * Stub:    does nothing.
 */
void hvb_ble_notify_error_message(const char *msg)
{
    (void)msg;
    // TODO:
}
