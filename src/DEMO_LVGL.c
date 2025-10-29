
// #include <Arduino.h>
#include <lvgl.h>
#include "display.h"
#include "esp_bsp.h"
#include "lv_port.h"
#include <esp_log.h>
#include <esp_flash.h>
#include <esp_chip_info.h>
#include <esp_system.h>
#include <esp_heap_caps.h>
#include "app_clock.h"

static const char *TAG = "DEMO_LVGL";

#define BUILD (String(__DATE__) + " - " + String(__TIME__)).c_str()

#define logSection(section) \
  ESP_LOGI(TAG, "\n\n************* %s **************\n", section);

/**
 * @brief LVGL porting example
 * Set the rotation degree:
 *      - 0: 0 degree
 *      - 90: 90 degree
 *      - 180: 180 degree
 *      - 270: 270 degree
 *
 */
#define LVGL_PORT_ROTATION_DEGREE (90)

/**
 * To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 * You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 */
 // #include <examples/lv_examples.h>

void setup();

#if !CONFIG_AUTOSTART_ARDUINO
void app_main()
{
  // initialize arduino library before we start the tasks
  // initArduino();

  setup();
}
#endif
void setup()
{
  //  String title = "LVGL porting example";

  // Serial.begin(115200);
  logSection("LVGL porting example start");
  esp_chip_info_t chip_info;
  uint32_t flash_size;
  esp_chip_info(&chip_info);
  ESP_LOGI(TAG, "This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

  unsigned major_rev = chip_info.revision / 100;
  unsigned minor_rev = chip_info.revision % 100;
  ESP_LOGI(TAG, "silicon revision v%d.%d, ", major_rev, minor_rev);
  if (esp_flash_get_size(NULL, &flash_size) != ESP_OK)
  {
    ESP_LOGI(TAG, "Get flash size failed");
    return;
  }

  ESP_LOGI(TAG, "%" PRIu32 "MB %s flash", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  ESP_LOGI(TAG, "Minimum free heap size: %" PRIu32 " bytes", esp_get_minimum_free_heap_size());
  size_t freePsram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
  ESP_LOGI(TAG, "Free PSRAM: %d bytes", freePsram);
  logSection("Initialize panel device");
  bsp_display_cfg_t cfg = {
      .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
      .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
#if LVGL_PORT_ROTATION_DEGREE == 90
      .rotate = LV_DISP_ROT_90,
#elif LVGL_PORT_ROTATION_DEGREE == 270
      .rotate = LV_DISP_ROT_270,
#elif LVGL_PORT_ROTATION_DEGREE == 180
      .rotate = LV_DISP_ROT_180,
#elif LVGL_PORT_ROTATION_DEGREE == 0
      .rotate = LV_DISP_ROT_NONE,
#endif
  };

  lv_disp_t *disp = bsp_display_start_with_config(&cfg);
  if (disp == NULL)
  {
    ESP_LOGE(TAG, "Failed to initialize display");
    return;
  }
  bsp_display_backlight_on();

  logSection("Load Elecrow clock UI");
  esp_err_t ui_err = app_clock_start(disp);
  if (ui_err != ESP_OK)
  {
    ESP_LOGE(TAG, "app_clock_start failed: %s", esp_err_to_name(ui_err));
    return;
  }

  logSection("LVGL porting example end");
}

void loop()
{
  ESP_LOGI(TAG, "IDLE loop");
  // delay(1000);
}