/**
 * @file home_handlers.h
 * @author Flavian Fernandes (flavian.fernandes@splice.com.br)
 * @brief 
 * @version 0.1
 * @date 2025-01-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __HOME_HANDLERS_H__
#define __HOME_HANDLERS_H__

#include "esp_err.h"
#include "esp_http_server.h"

extern const char get_home_uri[];
esp_err_t get_handler(httpd_req_t *req);

#if HAVE_FAVICON == 1
extern const char get_favicon_ico_uri[];
esp_err_t get_favicon_ico_handle(httpd_req_t *req);
#endif

#endif // __HOME_HANDLERS_H__