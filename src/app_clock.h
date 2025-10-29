#pragma once

#include "esp_err.h"
#include "lvgl.h"

/**
 * @brief Initialize the Elecrow desk clock UI on the active LVGL display.
 *
 * This function assumes LVGL has already been initialized through the BSP
 * helpers and that a display is registered. It will create the screen,
 * register event handlers, and perform an initial weather/time refresh.
 *
 * @param disp Pointer to the LVGL display returned by bsp_display_start_with_config.
 * @return ESP_OK on success, or an esp_err_t describing the failure.
 */
esp_err_t app_clock_start(lv_disp_t *disp);

/**
 * @brief Periodic service hook to be called from the main loop if needed.
 *
 * Currently this is a placeholder to keep parity with other application
 * modules. It can remain empty if no additional processing is required.
 */
void app_clock_loop(void);