#include "esp_wifi.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_smartconfig.h"

#include "lwip/err.h"
#include "lwip/sys.h"

void wifi_init_sta(void);
void vTaskSmartConfig(void);