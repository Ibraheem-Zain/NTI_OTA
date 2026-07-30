#pragma once

// C
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// ESP-IDF & Network
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_netif.h"

// Protocols
#include "mqtt_client.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"

// driver
#include "driver/gpio.h"

void start_ota_system(const char *ssid, const char *password, const char *server_ip, const char *broker_url);