#ifndef ADC_H
#define ADC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/dac.h"
#include "esp_system.h"

#include "../../MQTT/include/MQTT.h"
#include "../../../main/Init.h"

#define ADC_Calib 0.0003244

#define Resistor_1 3000000
#define Resistor_2 15000
#define Total_res ((Resistor_1+Resistor_2)/Resistor_2)/1.41

#define Voltage_Calib 2
#define Voltage_coefficient 0.17

#define Current_Calib 29.4

#define Zero_point 1.25

#define Range_data 32

#define ADC_CHANNEL_VOLTAGE_P   (ADC_CHANNEL_VOLTAGE_P_PIN-11)
#define ADC_CHANNEL_VOLTAGE_N   (ADC_CHANNEL_VOLTAGE_N_PIN-11)
#define ADC_CHANNEL_CURRENT     (ADC_CHANNEL_CURRENT_PIN-11)

typedef enum 
{
    UP,
    DOWN,
    INIT
}state_adc_t;

// typedef uint8_t state_adc_t;

void adc_init(void);
void vTaskADC2Conversation(void);

float raw_data_current[Range_data],raw_data_voltage_P[Range_data],raw_data_voltage_N[Range_data],TX_BUFFER_ADC[4];
state_adc_t state_adc;

#endif