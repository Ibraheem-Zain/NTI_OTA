#include "NTI_OTA_Libs.h"

static char target_server_ip[32];
static esp_mqtt_client_handle_t mqtt_client = NULL;

static void ota_task(void *pvParameter) {
    ESP_LOGI("OTA_LIB", "Starting OTA update from Node-RED...");

    char url[128];
    sprintf(url, "http://%s:1880/update.bin", target_server_ip);

    esp_http_client_config_t http_config = {
        .url = url,
        .keep_alive_enable = true,
    };
    esp_https_ota_config_t ota_config = {
        .http_config = &http_config,
    };

    ESP_LOGI("OTA_LIB", "Downloading and flashing from: %s", url);
    esp_err_t ret = esp_https_ota(&ota_config);

    if(ret == ESP_OK){
        ESP_LOGI("OTA_LIB", "SUCCESS! Flash written. Rebooting in 2 seconds...");
        vTaskDelay(pdMS_TO_TICKS(2000));
        esp_restart();
    }else{
        ESP_LOGE("OTA_LIB", "FAILED! Error: %s", esp_err_to_name(ret));
    }
    
    vTaskDelete(NULL);
}

// callback function
static void mqtt_receiver(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    if (event_id == MQTT_EVENT_CONNECTED) {
        esp_mqtt_client_subscribe(mqtt_client, "ESP32", 0);
    } 
    else if (event_id == MQTT_EVENT_DATA) {
        if (strncmp(event->data, "Update required", event->data_len) == 0) {
            xTaskCreate(&ota_task, "ota_task", 8192, NULL, 5, NULL);
        }
    }
}

// the OTA system booting
void start_ota_system(const char *ssid, const char *password, const char *server_ip, const char *broker_url) {
    
    strncpy(target_server_ip, server_ip, sizeof(target_server_ip) - 1);

    // open port, initiate wifi, and wait till connection
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_t *netif = esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {};
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    esp_wifi_connect();

    int wait_time = 0;
    esp_netif_ip_info_t ip_info;
    
    while (wait_time < 20){
        esp_netif_get_ip_info(netif, &ip_info);
        
        if(ip_info.ip.addr != 0){
            ESP_LOGI("OTA_LIB", "Wifi connection does succesfully!!");
            break; 
        }
        
        ESP_LOGI("OTA_LIB", "Wifi still connecting... %d", wait_time + 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        wait_time++;
    }

    if (wait_time >= 20){
        ESP_LOGE("OTA_LIB", "Wifi fail to connect!!");
        return;
    }

    ESP_LOGI("OTA_LIB", "Starting MQTT connection to: %s\n", broker_url);
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = broker_url,
    };
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_receiver, NULL);
    esp_mqtt_client_start(mqtt_client);
}