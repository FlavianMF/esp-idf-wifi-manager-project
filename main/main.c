/*
Copyright (c) 2017-2020 Tony Pottier

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

@file user_main.c
@author Tony Pottier
@brief Entry point for the ESP32 application.
@see https://idyl.io
@see https://github.com/tonyp7/esp32-wifi-manager
*/

#include <stdio.h>
#include <string.h>
#include <esp_wifi.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include "wifi_manager.h"
#include "http_app.h"

#include "ota_ws_update.h"


/* @brief tag used for ESP serial console messages */
static const char TAG[] = "main";


/*
 * This handler echos back the received ws data
 * and triggers an async send if certain message received
 */
static esp_err_t echo_handler(httpd_req_t *req)
{
    if (req->method == HTTP_GET) {
        ESP_LOGI(TAG, "Handshake done, the new connection was opened");
        return ESP_OK;
    }
    httpd_ws_frame_t ws_pkt;
    uint8_t *buf = NULL;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;
    /* Set max_len = 0 to get the frame len */
    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed to get frame len with %d", ret);
        return ret;
    }
    //ESP_LOGI(TAG, "frame len is %d", ws_pkt.len);
    if (ws_pkt.len) {
        /* ws_pkt.len + 1 is for NULL termination as we are expecting a string */
        buf = calloc(1, ws_pkt.len + 1);
        if (buf == NULL) {
            ESP_LOGE(TAG, "Failed to calloc memory for buf");
            return ESP_ERR_NO_MEM;
        }
        ws_pkt.payload = buf;
        /* Set max_len = ws_pkt.len to get the frame payload */
        ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
            free(buf);
            return ret;
        }
        //ESP_LOGI(TAG, "Got packet with message: %s", ws_pkt.payload);
    }
    //ESP_LOGI(TAG, "Packet type: %d", ws_pkt.type);

    ret = httpd_ws_send_frame(req, &ws_pkt);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_send_frame failed with %d", ret);
    }
    free(buf);
    return ret;
}

static const httpd_uri_t example_ws = {
        .uri        = "/ws",
        .method     = HTTP_GET,
        .handler    = echo_handler,
        .user_ctx   = NULL,
        .is_websocket = true
};

static esp_err_t get_handler(httpd_req_t *req)
{
    extern const unsigned char example_echo_ws_server_html_start[] asm("_binary_example_echo_ws_server_html_start");
    extern const unsigned char example_echo_ws_server_html_end[] asm("_binary_example_echo_ws_server_html_end");
    const size_t example_echo_ws_server_html_size = (example_echo_ws_server_html_end - example_echo_ws_server_html_start);

    httpd_resp_send_chunk(req, (const char *)example_echo_ws_server_html_start, example_echo_ws_server_html_size);
    httpd_resp_sendstr_chunk(req, NULL);
    return ESP_OK;
}
static const httpd_uri_t example_gh = {
    .uri = "/webc",
    .method = HTTP_GET,
    .handler = get_handler,
    .user_ctx = NULL
};

static esp_err_t my_get_handler(httpd_req_t *req){

	/* our custom page sits at /helloworld in this example */
	if(strcmp(req->uri, "/helloworld") == 0){

		ESP_LOGI(TAG, "Serving page /helloworld");

		const char* response = "<html><body><h1>Hello World!</h1></body></html>";

		httpd_resp_set_status(req, "200 OK");
		httpd_resp_set_type(req, "text/html");
		httpd_resp_send(req, response, strlen(response));
	}
	else if (strcmp(req->uri, "/webs") == 0)
	{
		get_handler(req);
	}
	// else if (strcmp(req->uri, "/ws") == 0)
	// {
	// 	echo_handler(req);
	// }
	else{
		/* send a 404 otherwise */
		httpd_resp_send_404(req);
	}

	return ESP_OK;
}

static const httpd_uri_t unregister_all = {
    .uri = "*",
    .method = HTTP_GET,
    .handler = NULL,
    .user_ctx = NULL
};

/**
 * @brief Callback to be called when the wifi manager connects to an access point.
 *
 * This callback is called when the wifi manager connects to an access point.
 * It is used to register the handlers for the web server.
 *
 * @note The callback is called from the wifi manager's task, so it should not block or
 *       cause any other tasks to block.
 *
 * @param pvParameters The parameter passed to the callback. It is not used in this
 *        case.
 */
void cb_connection_ok(void *pvParameters)
{
	ESP_LOGI(TAG, "Connection ok");

	httpd_handle_t wifi_manager_server = NULL;

	esp_err_t err = get_http_server_handle(&wifi_manager_server);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "get_http_server_handle failed (%s)!", esp_err_to_name(err));
	}
	else if (wifi_manager_server == NULL) {
		ESP_LOGE(TAG, "wifi_manager_server is NULL");
	}
	ESP_LOGI(TAG, "wifi_manager_server getted");

	// httpd_unregister_uri_handler(wifi_manager_server, unregister_all.uri);

	err = httpd_unregister_uri_handler(wifi_manager_server, unregister_all.uri, unregister_all.method);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "httpd_unregister_uri_handler failed (%s)!", esp_err_to_name(err));
	}
	ESP_LOGI(TAG, "wifi_manager_server \"*\" unregistered");

	err = httpd_register_uri_handler(wifi_manager_server, &example_gh);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "httpd_register_uri_handler failed (%s)!", esp_err_to_name(err));
	}

	err = httpd_register_uri_handler(wifi_manager_server, &example_ws);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "httpd_register_uri_handler failed (%s)!", esp_err_to_name(err));
	}

    ota_ws_register_uri_handler(wifi_manager_server);


	ESP_LOGI(TAG, "wifi_manager_server registered");


	ESP_LOGI(TAG, "Essa é uma nova versão");

	// wifi_manager_disconnect_async();

	return;
}

void app_main()
{
	/* start the wifi manager */
	wifi_manager_start();


	wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_connection_ok);


	/* set custom handler for the http server
	 * Now navigate to /helloworld to see the custom page
	 * */
	http_app_set_handler_hook(HTTP_GET, &my_get_handler);

}
