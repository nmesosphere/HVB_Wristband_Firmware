/*
 * HVB wristband firmware — entry point.
 *
 * Target: ESP32-C3-MINI-1, ESP-IDF + NimBLE. No Arduino APIs.
 */
#include "hvb_app.h"

/*
 * app_main — called once by ESP-IDF after boot. Returns nothing.
 *
 * Its only job is to bring the modules up in dependency order and then
 * return; all ongoing work happens in the app task (hvb_app) and the
 * NimBLE host task (hvb_ble). Intended order and why:
 *
 *   1. nvs_flash_init()   NimBLE needs NVS. If it reports
 *                         ESP_ERR_NVS_NO_FREE_PAGES / NEW_VERSION_FOUND,
 *                         erase and re-init.
 *   2. hvb_led_init()     LEDs must be in a known (off) state before
 *                         anything can try to show a color.
 *   3. hvb_storage_init() Show store must exist before a transfer can
 *                         finish or a show can be started.
 *   4. hvb_app_init()     Event queue + app task must exist BEFORE BLE
 *                         starts, because BLE callbacks post events to it.
 *   5. hvb_ble_init()     Registers the GATT service and starts
 *                         advertising; from here on the phone can connect.
 *
 * Any init failure should be logged and treated as fatal (ESP_ERROR_CHECK
 * or reboot), since the band is useless without all of these.
 *
 * The modules are still stubs that return ESP_OK, so this boots and
 * returns; the inits become real as each step is implemented.
 */
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "host/ble_uuid.h"   /* TEMP: UUID macro boot test */

#include "hvb_protocol.h"
#include "hvb_led.h"
#include "hvb_storage.h"
#include "hvb_ble.h"

static const char *TAG = "hvb_main";

/* NVS holds NimBLE's bonding/config data. A full or format-changed NVS
 * partition is recoverable by erasing it (we keep no user data there in v1). */
static void hvb_nvs_init(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS needs erase (%s), erasing", esp_err_to_name(err));
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void app_main(void){
    ESP_LOGI(TAG, "HVB boot fw=%u", (unsigned)HVB_FIRMWARE_VERSION);

    hvb_nvs_init();
    ESP_LOGI(TAG, "nvs ok");

    ESP_ERROR_CHECK(hvb_led_init());
    ESP_LOGI(TAG, "led ok");

    ESP_ERROR_CHECK(hvb_storage_init());
    ESP_LOGI(TAG, "storage ok");

    ESP_ERROR_CHECK(hvb_app_init());
    ESP_LOGI(TAG, "app ok");

    ESP_ERROR_CHECK(hvb_ble_init());
    ESP_LOGI(TAG, "ble ok");

    ESP_LOGI(TAG, "init complete");

    /* TEMP: UUID macro boot test — remove once verified */
    static const ble_uuid128_t svc_uuid = BLE_UUID128_INIT(HVB_UUID128_BYTES(HVB_UUID_SERVICE));
    static const ble_uuid128_t fw_uuid  = BLE_UUID128_INIT(HVB_UUID128_BYTES(HVB_UUID_FIRMWARE_VERSION));
    char buf[BLE_UUID_STR_LEN];
    ESP_LOGI(TAG, "uuid svc = %s", ble_uuid_to_str(&svc_uuid.u, buf));
    ESP_LOGI(TAG, "uuid fw  = %s", ble_uuid_to_str(&fw_uuid.u, buf));
}
