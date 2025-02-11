#include "home_handlers.h"

#include "esp_err.h"
#include "esp_http_server.h"

const char get_home_uri[] = "/";
esp_err_t get_handler(httpd_req_t *req)
{
    extern const unsigned char example_echo_ws_server_html_start[] asm("_binary_index_home_min_html_start");
    extern const unsigned char example_echo_ws_server_html_end[] asm("_binary_index_home_min_html_end");
    const size_t example_echo_ws_server_html_size = (example_echo_ws_server_html_end - example_echo_ws_server_html_start);

    httpd_resp_send_chunk(req, (const char *)example_echo_ws_server_html_start, example_echo_ws_server_html_size);
    httpd_resp_sendstr_chunk(req, NULL);
    return ESP_OK;
}

#if HAVE_FAVICON == 1
const char get_favicon_ico_uri[] = "/favicon.ico";
esp_err_t get_favicon_ico_handle(httpd_req_t *req)
{
    extern const unsigned char favicon_ico_start[] asm("_binary_favicon_ico_start");
    extern const unsigned char favicon_ico_end[] asm("_binary_favicon_ico_end");
    const size_t favicon_ico_size = (favicon_ico_end - favicon_ico_start);

    httpd_resp_set_type(req, "image/x-icon");
    httpd_resp_send(req, (const char *)favicon_ico_start, favicon_ico_size);
    return ESP_OK;
}
#endif