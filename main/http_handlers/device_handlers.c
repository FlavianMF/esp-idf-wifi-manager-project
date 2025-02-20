#include "device_handlers.h"

#include "freertos/FreeRTOS.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_timer.h"

#include "esp_http_server.h"

#include "esp_app_desc.h"
#include "spi_flash_mmap.h"
#include "esp_ota_ops.h"
#include "esp_mac.h"

#include "cJSON.h"

#include "version_info.h"

#include "wifi_manager.h"

static const char *TAG = "device_handlers";

const char get_app_version_uri[] = "/app_version";
esp_err_t app_version_get_handler(httpd_req_t *req)
{
    esp_app_desc_t app_desc;
    const esp_partition_t *running = esp_ota_get_running_partition();
   
    if (esp_ota_get_partition_description(running, &app_desc) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get partition description");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    cJSON *json_response = cJSON_CreateObject();
    if (json_response == NULL)
    {
        ESP_LOGE(TAG, "Failed to create JSON object");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    char version_string[50]; // Buffer para armazenar a versão
    snprintf(version_string, sizeof(version_string), "%d.%d.%d-%s-%d",
                    VERSION_MAJOR,
                    VERSION_MINOR,
                    PATCHLEVEL,
                    app_desc.version,
                    VERSION_TWEAK);

    ESP_LOGI(TAG, "App version string: %s", version_string);

    cJSON_AddStringToObject(json_response, "app_version", version_string);
    cJSON_AddStringToObject(json_response, "project_name", app_desc.project_name);
    cJSON_AddStringToObject(json_response, "idf_ver", app_desc.idf_ver);
    cJSON_AddStringToObject(json_response, "build_date", app_desc.date);
    cJSON_AddStringToObject(json_response, "build_time", app_desc.time);
    cJSON_AddNumberToObject(json_response, "secure_version", app_desc.secure_version);

    const char *response = cJSON_Print(json_response);
    if (response == NULL)
    {
        ESP_LOGE(TAG, "Failed to print JSON object");
        cJSON_Delete(json_response);
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "app_version json response: \n%s", response);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);

    cJSON_Delete(json_response);
    free((void *)response);

    return ESP_OK;    
}

const char get_chip_id_uri[] = "/chip_id";
esp_err_t chip_id_get_handler(httpd_req_t *req)
{
    uint8_t chipid[6];

    esp_err_t err = esp_efuse_mac_get_default(chipid);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_efuse_mac_get_default failed (%s)!", esp_err_to_name(err));
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    char chip_id_str[30];
    snprintf(chip_id_str, sizeof(chip_id_str), "%02X:%02X:%02X:%02X:%02X:%02X", chipid[0], chipid[1], chipid[2], chipid[3], chipid[4], chipid[5]);
    
    ESP_LOGI(TAG, "Chip ID: %s", chip_id_str);
    httpd_resp_send(req, chip_id_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}


const char get_reset_board_uri[] = "/reset_board";
esp_err_t reset_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "Restarting board...");

    httpd_resp_send(req, "Restarting board", HTTPD_RESP_USE_STRLEN);

    vTaskDelay(pdMS_TO_TICKS(2000));

    esp_restart();
    return ESP_OK;
}

const char get_uptime_uri[] = "/uptime";
esp_err_t uptime_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "Getting board uptime");

    uint64_t uptime_milliseconds = esp_timer_get_time() / 1000;

    char uptime_str[30];
    snprintf(uptime_str, sizeof(uptime_str), "%lld", uptime_milliseconds);
    ESP_LOGI(TAG, "Uptime string: %s", uptime_str);

    httpd_resp_send(req, uptime_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

const char get_reset_reason_uri[] = "/reset_reason";
esp_err_t reset_reason_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "Getting reset reason");
    esp_reset_reason_t reason = esp_reset_reason();
    char reason_str[30];
    snprintf(reason_str, sizeof(reason_str), "%d", reason);
    ESP_LOGI(TAG, "Reset reason: %s", reason_str);
    httpd_resp_send(req, reason_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

const char get_forget_wifi_credentials_uri[] = "/forget_wifi";
esp_err_t forget_wifi_credentials_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "forget_wifi_credentials");

    wifi_manager_disconnect_async();

    httpd_resp_sendstr(req, "Wifi credentials forgotten");

    return ESP_OK;
}