/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "../components/Wifi/include/Wifi.h"
#include "../components/MQTT/include/MQTT.h"
#include "../components/ADC/include/ADC.h"

#define BLINK_LED 2

void init_config(void)
{
    gpio_pad_select_gpio(BLINK_LED);
    gpio_set_direction(BLINK_LED,GPIO_MODE_OUTPUT);

    wifi_init_sta();
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    init_config();

    while(1)
    {
        // printf("Hi\n");
        // gpio_set_level(BLINK_LED,0);
        // vTaskDelay(100);
        // printf("Bye\n");
        // gpio_set_level(BLINK_LED,1);
        // vTaskDelay(100);
    }
}
