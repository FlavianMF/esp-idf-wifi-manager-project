#include "http_local_config.h"

#include "esp_err.h"
#include "esp_log.h"

#include "http_app.h"
#include "esp_http_server.h"
#include "wifi_manager.h"
#include "ota_ws_update.h"

#include "home_handlers.h"

static const char *TAG = "http_local_config";

typedef struct {
    const char *uri;
    esp_err_t (*handler)(httpd_req_t *req);
} http_handlers_t;

static const http_handlers_t get_http_handlers[] = {
    {
        .uri = get_home_uri,
        .handler = get_handler,
    },
};

// static const http_handlers_t post_http_handlers[] = {
//     {}
// };

esp_err_t all_get_handler(httpd_req_t *req)
{
    for (int i = 0; i < sizeof(get_http_handlers) / sizeof(get_http_handlers[0]); i++) {
        if (strcmp(req->uri, get_http_handlers[i].uri) == 0) {
            return get_http_handlers[i].handler(req);
        }
    }
    ESP_LOGE(TAG, "URI to GET not found");
    httpd_resp_send_404(req);    
    return ESP_OK;
}

esp_err_t all_post_handler(httpd_req_t *req)
{
    // for (int i = 0; i < sizeof(post_http_handlers) / sizeof(post_http_handlers[0]); i++)
    // {
    //     if (strcmp(req->uri, post_http_handlers[i].uri) == 0) {
    //         return post_http_handlers[i].handler(req);
    //     }
    // }

    ESP_LOGE(TAG, "URI to POST not found");
    httpd_resp_send_404(req);
    
    return ESP_OK;
}

void http_local_config(void *pvParameters)
{
    ESP_LOGI(TAG, "Initializing http local configurations");

    httpd_handle_t wifi_manager_server = NULL;

	esp_err_t err = get_http_server_handle(&wifi_manager_server);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "get_http_server_handle failed (%s)!", esp_err_to_name(err));
	}
	else if (wifi_manager_server == NULL) {
		ESP_LOGE(TAG, "wifi_manager_server is NULL");
	}
	ESP_LOGI(TAG, "wifi_manager_server getted");

    /** @note Unregister esp32_wifi_manager URIs */

    httpd_uri_t http_uri = {
        .uri = "*",
        .method = HTTP_GET,
        .handler = NULL,
        .user_ctx = NULL
    };

    err = httpd_unregister_uri_handler(wifi_manager_server, http_uri.uri, http_uri.method);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "httpd_unregister_uri_handler failed (%s)!", esp_err_to_name(err));
	}
	ESP_LOGI(TAG, "wifi_manager_server \"*\" GET unregistered");

    http_uri.method = HTTP_POST;

    err = httpd_unregister_uri_handler(wifi_manager_server, http_uri.uri, http_uri.method);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "httpd_unregister_uri_handler failed (%s)!", esp_err_to_name(err));
	}
	ESP_LOGI(TAG, "wifi_manager_server \"*\" GET unregistered");

    /** @note register new URI */

    err = ota_ws_register_uri_handler(wifi_manager_server);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "ota_ws_register_uri_handler failed (%s)!", esp_err_to_name(err));
    }
    ESP_LOGI(TAG, "ota_ws_register_uri_handler registered");

    http_uri.method = HTTP_GET;
    http_uri.handler = all_get_handler;

	err = httpd_register_uri_handler(wifi_manager_server, &http_uri);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "httpd_register_uri_handler failed (%s)!", esp_err_to_name(err));
	}
    ESP_LOGI(TAG, "Registered GET uri handler");

    http_uri.method = HTTP_POST;
    http_uri.handler = all_post_handler;

	err = httpd_register_uri_handler(wifi_manager_server, &http_uri);
	if (err != ESP_OK)
    {
		ESP_LOGE(TAG, "httpd_register_uri_handler failed (%s)!", esp_err_to_name(err));
	}
    ESP_LOGI(TAG, "Registered POST uri handler");

	ESP_LOGI(TAG, "wifi_manager_server registered");
    return;
}