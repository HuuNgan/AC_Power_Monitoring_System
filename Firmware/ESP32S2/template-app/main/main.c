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

#include "Init.h"

#include <math.h>

#define BUFFER_SIZE     Range_data/2
#define SCAN_RANGE      1

float maxima_array[];
float wattage=0;
uint64_t power_consumption=0;
uint16_t counter=0;

void getRAMinfo(void)
{
    int heap_size=0;
    heap_size=heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
    printf("Heap size: %d\n",heap_size);
    heap_size=heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    printf("Heap free size: %d\n",heap_size);
    vTaskDelay(100);
}

int maxima(float *buffer)
{
    int i, j, maxima_pos=0;
    float max_value=0;
    for(i=0; i< BUFFER_SIZE; i++)
    {
        if(buffer[i] < 0) buffer[i] = -buffer[i];
    }
    for(i = SCAN_RANGE; i < BUFFER_SIZE-SCAN_RANGE; i++)
    {
        for(j=i-SCAN_RANGE; j <= i+SCAN_RANGE; j++)
        {
            if(max_value < buffer[j])   max_value = buffer[j];
        }
        if(max_value == buffer[i])
        {
            maxima_array[maxima_pos] = buffer[i];
            maxima_pos++;
        }
        max_value = 0;
    }
    return maxima_pos;          //return number of elements in maxima array
}

void vTaskCalculate(void)
{
    int max_data_position[3];
    for (;;)
    {
        if(state_adc==UP)
        {
            //code
            max_data_position[0]=maxima(raw_data_current);
            max_data_position[1]=maxima(raw_data_voltage_P);
            // printf("Voltage max: %f \n",raw_data_voltage_P[max_data_position[1]]);
        }
        else if(state_adc==DOWN)
        {
            //code
            max_data_position[0]=maxima(raw_data_current+BUFFER_SIZE);
            max_data_position[1]=maxima(raw_data_voltage_P+BUFFER_SIZE);
            // printf("Voltage max: %f \n",raw_data_voltage_P[max_data_position[1]]);
        }
        wattage=abs(raw_data_current[max_data_position[0]]*raw_data_voltage_P[max_data_position[1]])*cos(alpha_delay);
        Timer_power_consumption(&power_consumption,&wattage);
        counter++;
        if(counter>10000)
        {
            //sendmqtt
            vTaskMQTTPublish(raw_data_current[max_data_position[0]],raw_data_voltage_P[max_data_position[1]],alpha_delay,wattage,power_consumption);
            counter=0;
        }
        // printf("Power usage: %f \n",wattage);

        #if Time_dilation
        vTaskDelay(Time_dilation);
        #endif
    }
}

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

    getRAMinfo();

    init_config();
    vTaskDelay(500);

    xTaskCreate(vTaskADC2Conversation,"ADC Conversation",4096,NULL,1,NULL);
    xTaskCreate(vTaskCalculate,"Calculate Process",4096,NULL,2,NULL);
    xTaskCreate(vTaskGPIO_ISR_Process,"GPIO Process",4096,NULL,3,NULL);

    while(1)
    {
        vTaskDelay(100);
    }
}
