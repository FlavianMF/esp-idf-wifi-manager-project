#include "wifi_handlers.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"

#include "nvs_flash.h"
#include "nvs.h"

#include "esp_http_server.h"

#include "cJSON.h"

#include "static_ip_config.h"

static const char *TAG = "wifi_handlers";

const char get_wifi_config_uri[] = "/wifi_config";
esp_err_t get_wifi_config_handler(httpd_req_t *req)
{
    esp_err_t err = ESP_FAIL;
    ESP_LOGI(TAG, "Get Wifi Config");

    char **ip_str = NULL;
    err = load_static_ip_address(&ip_str);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to load ip address, (%s)", esp_err_to_name(err));
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    char **mask_str = NULL;
    err = load_static_ip_mask(&mask_str);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to load ip mask, (%s)", esp_err_to_name(err));
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    char **gateway_str = NULL;
    err = load_static_ip_gateway(&gateway_str);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to load gateway, (%s)", esp_err_to_name(err));
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "ip: %s, mask: %s, gateway: %s", ip_str, mask_str, gateway_str);

    cJSON *json_response = cJSON_CreateObject();
    if (json_response == NULL)
    {
        ESP_LOGE(TAG, "Failed to create JSON object");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    cJSON_AddStringToObject(json_response, "ip", ip_str);
    cJSON_AddStringToObject(json_response, "mask", mask_str);
    cJSON_AddStringToObject(json_response, "gateway", gateway_str);

    const char *response = cJSON_Print(json_response);
    if (response == NULL)
    {
        ESP_LOGE(TAG, "Failed to create JSON string");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "JSON response: \n%s", response);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    
    cJSON_Delete(json_response);

    free((void *)response);
    free(ip_str);
    free(mask_str);
    free(gateway_str);

    return ESP_OK;
 
}

const char post_wifi_config_uri[] = "/wifi_config";
esp_err_t post_wifi_config_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "Post Wifi Config");

    char *content = (char *)malloc(req->content_len + 1);
    if (content == NULL)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for content");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    int ret = httpd_req_recv(req, content, req->content_len);
    if (ret <= 0)
    {
        ESP_LOGE(TAG, "Failed to read request content");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    cJSON *json = cJSON_Parse(content);
    if (json == NULL)
    {
        ESP_LOGE(TAG, "Failed to parse JSON");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    cJSON *ip = cJSON_GetObjectItem(json, "ip");
    cJSON *mask = cJSON_GetObjectItem(json, "mask");
    cJSON *gateway = cJSON_GetObjectItem(json, "gateway");

    if (ip == NULL || mask == NULL || gateway == NULL)
    {
        ESP_LOGE(TAG, "Failed to get ip, mask or gateway from JSON");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "ip: %s", ip->valuestring);
    ESP_LOGI(TAG, "mask: %s", mask->valuestring);
    ESP_LOGI(TAG, "gateway: %s", gateway->valuestring);

    esp_err_t err = save_static_ip_to_nvs(ip->valuestring, mask->valuestring, gateway->valuestring);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to save static ip config, (%s)", esp_err_to_name(err));
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    cJSON_Delete(json);
    free(content);

    httpd_resp_send(req, NULL, 0);

    vTaskDelay(pdMS_TO_TICKS(3000));
    esp_restart();

    return ESP_OK;
}