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

#define Voltage_Calib 200
#define Current_Calib 29.4

#define Zero_point 1.25

#define Range_data 10

#define ADC_CHANNEL_VOLTAGE_P   2
#define ADC_CHANNEL_VOLTAGE_N   3
#define ADC_CHANNEL_CURRENT     0

void adc_init(void);
void vTaskADC2Conversation(void);