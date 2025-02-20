#include "static_ip_config.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_netif.h"
#include <netdb.h>

#include "nvs_flash.h"
#include "nvs.h"

#include "lwip/ip_addr.h"
#include "lwip/ip4_addr.h"

#include "wifi_manager.h"

static const char *TAG = "static_ip_config";

esp_err_t load_static_ip_address(char **ip_str)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("wifi", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "nvs_open failed (%s)!", esp_err_to_name(err));
        return err;
    }

    size_t required_str_size = 0;

    nvs_get_str(nvs_handle, "ip", NULL, &required_str_size);
    *ip_str = (char *)malloc(required_str_size);
    err = nvs_get_str(nvs_handle, "ip", *ip_str, &required_str_size);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "nvs_get_str failed to get ip (%s)!", esp_err_to_name(err));
        free(*ip_str);
        return err;    
    }
    ESP_LOGI(TAG, "ip: %s", *ip_str);
    return err;
}

esp_err_t load_static_ip_mask(char **mask_str)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("wifi", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "nvs_open failed (%s)!", esp_err_to_name(err));
        return err;    
    }

    size_t required_str_size = 0;

    nvs_get_str(nvs_handle, "mask", NULL, &required_str_size);
    *mask_str = (char *)malloc(required_str_size);
    err = nvs_get_str(nvs_handle, "mask", *mask_str, &required_str_size);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "nvs_get_str failed to get mask (%s)!", esp_err_to_name(err));
        free(*mask_str);
        return err;    
    }
    ESP_LOGI(TAG, "mask: %s", *mask_str);
    return err;
}

esp_err_t load_static_ip_gateway(char **gateway_str)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("wifi", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "nvs_open failed (%s)!", esp_err_to_name(err));
        return err;    
    }

    size_t required_str_size = 0;

    nvs_get_str(nvs_handle, "gateway", NULL, &required_str_size);
    *gateway_str = (char *)malloc(required_str_size);
    err = nvs_get_str(nvs_handle, "gateway", *gateway_str, &required_str_size);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "nvs_get_str failed to get gateway (%s)!", esp_err_to_name(err));
        free(*gateway_str);
        return err;    
    }
    ESP_LOGI(TAG, "gateway: %s", *gateway_str);
    return err;
}

esp_err_t save_static_ip_to_nvs(char *ip_str, char *mask_str, char *gateway_str)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("wifi", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "nvs_open failed (%s)!", esp_err_to_name(err));
        return err;    
    }

    err = nvs_set_str(nvs_handle, "ip", ip_str);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "nvs_set_str failed to set ip (%s)!", esp_err_to_name(err));
        return err;    
    }

    err = nvs_set_str(nvs_handle, "mask", mask_str);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "nvs_set_str failed to set mask (%s)!", esp_err_to_name(err));
        return err;    
    }

    err = nvs_set_str(nvs_handle, "gateway", gateway_str);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "nvs_set_str failed to set gateway (%s)!", esp_err_to_name(err));
        return err;    
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "nvs_commit failed (%s)!", esp_err_to_name(err));
        return err;    
    }

    return err;
}

esp_err_t load_static_ip_from_nvs(esp_netif_ip_info_t *ip_info)
{
    esp_err_t err = ESP_FAIL;

    nvs_handle_t nvs_handle;
    err = nvs_open("wifi", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "nvs_open failed (%s)!", esp_err_to_name(err));
        return err;    
    }

    uint8_t magic_byte = 0;
    err = nvs_get_u8(nvs_handle, "magic", &magic_byte);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "nvs_get_u8 failed to get magic byte (%s)!", esp_err_to_name(err));
    }

    if (err == ESP_ERR_NVS_NOT_FOUND || magic_byte != 0x42)
    {
        ESP_LOGW(TAG, "magic byte is not 0x42");
        ESP_LOGW(TAG, "Using default values");

        magic_byte = 0x42;
        err = nvs_set_u8(nvs_handle, "magic", magic_byte);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to save magic number, (%s)", esp_err_to_name(err));
            return err;
        }

        char ip_str[] = ESP_DEFAULT_STATIC_IP_ADDRESS;
        char mask_str[] = ESP_DEFAULT_STATIC_IP_MASK;
        char gateway_str[] = ESP_DEFAULT_STATIC_IP_GATEWAY;

        err = nvs_set_str(nvs_handle, "ip", ip_str);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to save ip, (%s)", esp_err_to_name(err));
        }

        err = nvs_set_str(nvs_handle, "mask", mask_str);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to save mask, (%s)", esp_err_to_name(err));
        }

        err = nvs_set_str(nvs_handle, "gateway", gateway_str);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to save gateway, (%s)", esp_err_to_name(err));
        }

        ip_info->ip.addr = ipaddr_addr(ip_str);
        ip_info->netmask.addr = ipaddr_addr(mask_str);
        ip_info->gw.addr = ipaddr_addr(gateway_str);

        return ESP_OK;
    }
    else
    {
        ESP_LOGI(TAG, "Magic number already saved");

        size_t required_str_size = 0;

        nvs_get_str(nvs_handle, "ip", NULL, &required_str_size);
        char *ip_str = (char *)malloc(required_str_size);
        err = nvs_get_str(nvs_handle, "ip", ip_str, &required_str_size);
        if (err != ESP_OK) 
        {
            ESP_LOGE(TAG, "nvs_get_str failed to get ip (%s)!", esp_err_to_name(err));
            return err;    
        }
        ESP_LOGI(TAG, "ip: %s", ip_str);
        
        nvs_get_str(nvs_handle, "mask", NULL, &required_str_size);
        char *mask_str = (char *)malloc(required_str_size);
        err = nvs_get_str(nvs_handle, "mask", mask_str, &required_str_size);
        if (err != ESP_OK) 
        {
            ESP_LOGE(TAG, "nvs_get_str failed to get mask(%s)!", esp_err_to_name(err));
            return err;    
        }
        ESP_LOGI(TAG, "mask: %s", mask_str);
        
        nvs_get_str(nvs_handle, "gateway", NULL, &required_str_size);
        char *gateway_str = (char *)malloc(required_str_size);
        err = nvs_get_str(nvs_handle, "gateway", gateway_str, &required_str_size);
        if (err != ESP_OK)
        {
            ESP_LOGI(TAG, "nvs_get_str failed to get gateway (%s)!", esp_err_to_name(err));
            return err;
        }
        ESP_LOGI(TAG, "gateway: %s", gateway_str);

        ip_info->ip.addr = ipaddr_addr(ip_str);
        ip_info->netmask.addr = ipaddr_addr(mask_str);
        ip_info->gw.addr = ipaddr_addr(gateway_str);

        free(ip_str);
        free(mask_str);
        free(gateway_str);

        return ESP_OK;
    }
}

void http_local_config_static_ip(void *pvParameters)
{
    esp_err_t err = ESP_FAIL;
    ESP_LOGI(TAG, "Initializing http local STATIC IP");

    esp_netif_t *sta_netif = wifi_manager_get_esp_netif_sta();
    if(sta_netif == NULL)
    {
        ESP_LOGE(TAG, "Failed to get sta netif");
        return;
    }

    err = esp_netif_dhcpc_stop(sta_netif);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to stop dhcp client, (%s)", esp_err_to_name(err));
        return;
    }

    esp_netif_ip_info_t ip_info;
    memset(&ip_info, 0, sizeof(esp_netif_ip_info_t));

    err = load_static_ip_from_nvs(&ip_info);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "load_static_ip_from_nvs failed (%s)!", esp_err_to_name(err));
        
        ip_info.ip.addr = ipaddr_addr(ESP_DEFAULT_STATIC_IP_ADDRESS);
        ip_info.netmask.addr = ipaddr_addr(ESP_DEFAULT_STATIC_IP_MASK);
        ip_info.gw.addr = ipaddr_addr(ESP_DEFAULT_STATIC_IP_GATEWAY);
    }

    err = esp_netif_set_ip_info(sta_netif, &ip_info);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_netif_set_ip_info failed (%s)!", esp_err_to_name(err));
        return;
    }
    ESP_LOGI(TAG, "Success to set static ip: %s, netmask: %s, gw: %s", ip4addr_ntoa(ip_info.ip.addr), ip4addr_ntoa(ip_info.netmask.addr), ip4addr_ntoa(ip_info.gw.addr));
    
    return;
}