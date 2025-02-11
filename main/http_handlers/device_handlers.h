/**
 * @file device_handlers.h
 * @author Flavian Melquiades (flavian.melquiades@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-02-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __DEVICE_HANDLERS_H__
#define __DEVICE_HANDLERS_H__

#include "esp_err.h"
#include "esp_http_server.h"

extern const char get_app_version_uri[];
esp_err_t app_version_get_handler(httpd_req_t *req);

extern const char get_chip_id_uri[];
esp_err_t chip_id_get_handler(httpd_req_t *req);

#endif // __DEVICE_HANDLERS_H__