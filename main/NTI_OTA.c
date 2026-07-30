#include "NTI_OTA_Libs.h"

#define WIFI_SSID "WIFI_NETWORK_NAME"
#define WIFI_PASS "WIFI_NETWORK_PASSWORD"
#define NODE_RED_IP "SERVER_IP"
#define BROKER_URL "BROKER_IP"

void led_blink_task(void *pvParameter){
    int8_t led_pin = 2;
    int32_t delay_ms = 2000;

    gpio_reset_pin(led_pin);
    gpio_set_direction(led_pin, GPIO_MODE_OUTPUT);
    
    int8_t led_state = 0;
    while (1){
        gpio_set_level(led_pin, led_state);
        ESP_LOGI("APP", "LED State: %d | Delay: %d ms", led_state, delay_ms);
        led_state ^= 1;
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

void app_main(void) {
    ESP_LOGI("APP", "Booting NTI_OTA App...");

    xTaskCreate(&led_blink_task, "led_blink_task", 2048, NULL, 5, NULL);

    start_ota_system(WIFI_SSID, WIFI_PASS, NODE_RED_IP, BROKER_URL);

    while (1){ 
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
}