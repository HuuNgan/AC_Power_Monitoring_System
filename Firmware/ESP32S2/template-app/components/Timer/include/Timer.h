#ifndef TIME_H
#define TIME_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "esp_timer.h"

#include "../../../main/Init.h"
#include "../../GPIO/include/GPIO.h"

#define Clock_divider   40000
#define Max_timer       18446744073709551615
#define Calib_phase     (360/0.02)
#define DEGtoRAD        (3.14/180)

volatile esp_timer_handle_t periodic_timer;
volatile esp_timer_handle_t oneshot_timer;

void Timer_phase_init(void);
void Timer_phase_cal(bool state);
void Timer_power_consumption(uint64_t *power_consumption,float *wattage);

float alpha_delay;

#endif