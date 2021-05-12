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
#include "../components/Timer/include/Timer.h"
#include "../components/SmartConfig/include/SmartConfig.h"
#include "../components/GPIO/include/GPIO.h"

void init_config(void)
{
    GPIO_init();
    wifi_init_sta();
    adc_init();
    Timer_phase_init();
    MQTT_init();
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
    vTaskDelay(500);

    while(1)
    {
        vTaskADC2Conversation();
        // printf("Hello world\n");
        // gpio_set_level(BLINK_LED,0);
        vTaskDelay(50);
        // printf("Bye\n");
        // gpio_set_level(BLINK_LED,1);
        // vTaskDelay(100);
    }
}
