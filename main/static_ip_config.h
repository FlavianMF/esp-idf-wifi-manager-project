/**
 * @file static_ip_config.h
 * @author Flavian Fernandes (flavian.fernandes@splice.com.br)
 * @brief 
 * @version 0.1
 * @date 2025-02-17
 * 
 * @copyright SPLICE (c) 2025
 * 
 */
#ifndef __STATIC_IP_CONFIG_H__
#define __STATIC_IP_CONFIG_H__

#include "esp_err.h"
#include "esp_netif.h"

#if CONFIG_USE_STATIC_IP_CONFIGURATION == 1
#define ESP_DEFAULT_STATIC_IP_ADDRESS       CONFIG_DEFAULT_IP_ADDRESS
#define ESP_DEFAULT_STATIC_IP_MASK          CONFIG_DEFAULT_NETMASK
#define ESP_DEFAULT_STATIC_IP_GATEWAY       CONFIG_DEFAULT_GATEWAY
#else // CONFIG_USE_STATIC_IP_CONFIGURATION
#define ESP_DEFAULT_STATIC_IP_ADDRESS       "0.0.0.0"
#define ESP_DEFAULT_STATIC_IP_MASK          "0.0.0.0"
#define ESP_DEFAULT_STATIC_IP_GATEWAY       "0.0.0.0"
#endif // CONFIG_USE_STATIC_IP_CONFIGURATION

esp_err_t load_static_ip_address(char **ip_str);

esp_err_t load_static_ip_mask(char **mask_str);

esp_err_t load_static_ip_gateway(char **gateway_str);

esp_err_t save_static_ip_to_nvs(char *ip_str, char *mask_str, char *gateway_str);

esp_err_t load_static_ip_from_nvs(esp_netif_ip_info_t *ip_info);

void http_local_config_static_ip(void *pvParameters);

#endif // __STATIC_IP_CONFIG_H__