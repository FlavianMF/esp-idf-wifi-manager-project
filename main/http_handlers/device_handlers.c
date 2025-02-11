#include "device_handlers.h"

#include "esp_err.h"
#include "esp_log.h"

#include "esp_http_server.h"

#include "esp_app_desc.h"
#include "spi_flash_mmap.h"
#include "esp_ota_ops.h"
#include "esp_mac.h"

#include "cJSON.h"

#include "version_info.h"

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