#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "../../Timer/include/Timer.h"

#define BLINK_LED               (2)
#define BLINK_LED_PIN_SEL       (1ULL<<BLINK_LED)
#define BUTTON                  (4)
#define BUTTON_PIN_SEL          (1ULL<<BUTTON)
#define ESP_INTR_FLAG_DEFAULT   (0)

void GPIO_init(void);
void control_built_in_led(bool control);
void vTaskGPIO_ISR_Process(void* arg);
void IRAM_ATTR GPIO_ISR_Handler(void* arg);

#endif