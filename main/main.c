/**
 * @file main.c
 * @author Flavian Melquiades (flavian.melquiades@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <stdio.h>
#include <string.h>
#include <esp_wifi.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include "wifi_manager.h"

#include "http_local_config.h"

/** @brief tag used for ESP serial console messages */
static const char TAG[] = "main";


void app_main()
{
	/* start the wifi manager */
	/* start the wifi manager */
	wifi_manager_start();
    ESP_LOGI(TAG, "WiFi manager started");

	wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &http_local_config);
    ESP_LOGI(TAG, "WiFi manager callback setted");
}
