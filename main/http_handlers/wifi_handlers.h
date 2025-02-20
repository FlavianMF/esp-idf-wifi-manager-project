/**
 * @file wifi_handlers.h
 * @author Flavian Fernandes (flavian.fernandes@splice.com.br)
 * @brief 
 * @version 0.1
 * @date 2025-02-17
 * 
 * @copyright SPLICE (c) 2025
 * 
 */
#ifndef __WIFI_HANDLERS_H__
#define __WIFI_HANDLERS_H__

#include "esp_err.h"
#include "esp_http_server.h"

extern const char get_wifi_config_uri[];
esp_err_t get_wifi_config_handler(httpd_req_t *req);

extern const char post_wifi_config_uri[];
esp_err_t post_wifi_config_handler(httpd_req_t *req);

#endif // __WIFI_HANDLERS_H__