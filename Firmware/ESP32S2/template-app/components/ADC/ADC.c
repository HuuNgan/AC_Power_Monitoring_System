#include <stdio.h>
#include "include/ADC.h"

esp_err_t adc_channel[3];
static const adc_bits_width_t width = ADC_WIDTH_BIT_13;
float raw_data_current[Range_data],raw_data_voltage_P[Range_data],raw_data_voltage_N[Range_data],TX_BUFFER_ADC[4];

state_adc_t state_adc = INIT;

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

float sum_array(float *start_p,float *end_p, float init)
{
    float sum = init;
    for (size_t i = 0; i < end_p-start_p; i++)
    {
        sum+=*(start_p+i);
    }
    return sum;
}

void vTaskADC2Conversation(void)
{
    int read_raw=0;
    static uint8_t cnt=0;
    for(;;)
    {
        if(cnt==Range_data)
        {
            state_adc=DOWN;
            cnt=0;
        }
        else if(cnt==Range_data/2)
        {
            state_adc=UP;
        }

        for (int i = 0; i < 3; i++)
        {
            switch (i)
            {
            case 0:
                adc_channel[i] = adc2_get_raw( ADC_CHANNEL_VOLTAGE_P, width, &read_raw);
                raw_data_voltage_P[cnt]=(((float)read_raw*ADC_Calib-Zero_point)*Voltage_Calib/Voltage_coefficient)*Total_res*0.1;
                break;
            case 1:
                adc_channel[i] = adc2_get_raw( ADC_CHANNEL_VOLTAGE_N, width, &read_raw);
                raw_data_voltage_N[cnt]=(((float)read_raw*ADC_Calib-Zero_point)*Voltage_Calib/Voltage_coefficient)*Total_res*0.1;
                break;
            case 2:
                adc_channel[i] = adc2_get_raw( ADC_CHANNEL_CURRENT, width, &read_raw);
                raw_data_current[cnt]=((float)read_raw*ADC_Calib-Zero_point)*Current_Calib;
                break;
            default:
                break;
            }
            
            if ( adc_channel[i] == ESP_OK ) 
            {
                #if DEBUG_USER_ADC
                printf("ADC %d: %d, Voltage value: %f\n", i, read_raw, TX_BUFFER_ADC[0]);
                #endif
            } 
            else if ( adc_channel[i] == ESP_ERR_INVALID_STATE ) 
            {
                printf("%s: ADC2 not initialized yet.\n", esp_err_to_name(adc_channel[i]));
            } 
            else if ( adc_channel[i] == ESP_ERR_TIMEOUT ) 
            {
                //This can not happen in this example. But if WiFi is in use, such error code could be returned.
                printf("%s: ADC2 is in use by Wi-Fi.\n", esp_err_to_name(adc_channel[i]));
            } 
            else 
            {
                printf("%s\n", esp_err_to_name(adc_channel[i]));
            }
        }

        cnt++;
    #if Time_dilation
        vTaskDelay(Time_dilation);
    #endif
    }
    // vTaskMQTTPublish(raw_data);
}
