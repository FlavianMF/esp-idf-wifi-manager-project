#include "mdns_config.h"

#include "esp_err.h"
#include "esp_log.h"

#include "sdkconfig.h"

#include "mdns.h"

#define HOSTNAME_CONFIG CONFIG_LWIP_LOCAL_HOSTNAME
#define INSTANCENAME_CONFIG CONFIG_LWIP_LOCAL_HOSTNAME

static const char *TAG = "mds_config";

esp_err_t mdns_config()
{
    ESP_LOGI(TAG, "Initilizing the mdns");

    esp_err_t err = mdns_init();
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "Failed to initialize the mdns, (%s)", esp_err_to_name(err));
        return err;
    }

    err = mdns_hostname_set(HOSTNAME_CONFIG);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "mdns failed to set hostname, (%s)", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "mdns hostname set to: %s", HOSTNAME_CONFIG);

    err = mdns_instance_name_set(INSTANCENAME_CONFIG);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "mdns failed to set the instance name, (%s)", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "mdns instance name set to: %s", INSTANCENAME_CONFIG);

    return ESP_OK;
}
