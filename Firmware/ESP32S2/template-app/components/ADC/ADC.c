#include <stdio.h>
#include "include/ADC.h"

esp_err_t adc_channel[3];
static const adc_bits_width_t width = ADC_WIDTH_BIT_13;

void adc_init(void)
{
    gpio_num_t adc_gpio_num[3];

    adc2_pad_get_io_num(ADC_CHANNEL_VOLTAGE_P,adc_gpio_num);
    adc2_pad_get_io_num(ADC_CHANNEL_VOLTAGE_N,adc_gpio_num+1);
    adc2_pad_get_io_num(ADC_CHANNEL_CURRENT,adc_gpio_num+2);

    printf("ADC2 channel %d @ GPIO %d.\n", ADC_CHANNEL_VOLTAGE_P, adc_gpio_num[0]);
    printf("ADC2 channel %d @ GPIO %d.\n", ADC_CHANNEL_VOLTAGE_N, adc_gpio_num[1]);
    printf("ADC2 channel %d @ GPIO %d.\n", ADC_CHANNEL_CURRENT, adc_gpio_num[2]);

    adc2_config_channel_atten(ADC_CHANNEL_VOLTAGE_P,ADC_ATTEN_11db);
    adc2_config_channel_atten(ADC_CHANNEL_VOLTAGE_N,ADC_ATTEN_11db);
    adc2_config_channel_atten(ADC_CHANNEL_CURRENT,ADC_ATTEN_11db);

    vTaskDelay(100);
}

void vTaskADC2Conversation(void)
{
    int read_raw=0;
    float voltage_data[3],raw_data[3];
    for (int i = 0; i < 3; i++)
    {
        switch (i)
        {
        case 0:
            adc_channel[i] = adc2_get_raw( ADC_CHANNEL_VOLTAGE_P, width, &read_raw);
            break;
        case 1:
            adc_channel[i] = adc2_get_raw( ADC_CHANNEL_VOLTAGE_N, width, &read_raw);
            break;
        case 2:
            adc_channel[i] = adc2_get_raw( ADC_CHANNEL_CURRENT, width, &read_raw);
            break;
        default:
            break;
        }
        
        if ( adc_channel[i] == ESP_OK ) 
        {
            voltage_data[i]=(float)read_raw*ADC_Calib-1.25;
            if(i==0)
                raw_data[i]=voltage_data[i]*Current_Calib;
            else
                raw_data[i]=voltage_data[i]*Voltage_Calib;
            #if DEBUG_ADC
            printf("ADC %d: %d, Voltage value: %f\n", i, read_raw, raw_data[i] );
            #endif
        } else if ( adc_channel[i] == ESP_ERR_INVALID_STATE ) 
        {
            printf("%s: ADC2 not initialized yet.\n", esp_err_to_name(adc_channel[i]));
        } else if ( adc_channel[i] == ESP_ERR_TIMEOUT ) 
        {
            //This can not happen in this example. But if WiFi is in use, such error code could be returned.
            printf("%s: ADC2 is in use by Wi-Fi.\n", esp_err_to_name(adc_channel[i]));
        } else 
        {
            printf("%s\n", esp_err_to_name(adc_channel[i]));
        }
    }
    vTaskMQTTPublish(raw_data);
}
