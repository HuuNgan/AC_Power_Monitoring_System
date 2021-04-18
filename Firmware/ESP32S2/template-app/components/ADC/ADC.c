#include <stdio.h>
#include "include/ADC.h"

esp_err_t r;
static const adc_bits_width_t width = ADC_WIDTH_BIT_13;

void adc_init(void)
{
    gpio_num_t adc_gpio_num;

    adc2_pad_get_io_num(ADC_CHANNEL_VOLTAGE_P,&adc_gpio_num);

    printf("ADC2 channel %d @ GPIO %d.\n", ADC_CHANNEL_VOLTAGE_P, adc_gpio_num);
    adc2_config_channel_atten(ADC_CHANNEL_VOLTAGE_P,ADC_ATTEN_11db);
    vTaskDelay(10);
}

void vTaskADC2Conversation(void)
{
    uint8_t output_data=0;
    int read_raw;
    float voltage_data=0;
    r = adc2_get_raw( ADC_CHANNEL_VOLTAGE_P, width, &read_raw);
    if ( r == ESP_OK ) 
    {
        voltage_data=(float)read_raw*ADC_Calib;
        printf("ADC %d: %d, Voltage value: %f\n", output_data, read_raw, voltage_data );
    } else if ( r == ESP_ERR_INVALID_STATE ) 
    {
        printf("%s: ADC2 not initialized yet.\n", esp_err_to_name(r));
    } else if ( r == ESP_ERR_TIMEOUT ) 
    {
        //This can not happen in this example. But if WiFi is in use, such error code could be returned.
        printf("%s: ADC2 is in use by Wi-Fi.\n", esp_err_to_name(r));
    } else 
    {
        printf("%s\n", esp_err_to_name(r));
    }
    vTaskMQTTSend(voltage_data);
}
