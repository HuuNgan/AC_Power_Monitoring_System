#ifndef SMART_CONFIG_H
#define SMART_CONFIG_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_smartconfig.h"
#include "esp_wifi.h"

#include "../../Wifi/include/Wifi.h"
#include "../../SPI_Flash/include/SPI_Flash.h"

void vTaskSmartConfig(void);
void vTaskSmartConfigStart(void);

#endif