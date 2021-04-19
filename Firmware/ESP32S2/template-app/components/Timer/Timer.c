#include <stdio.h>
#include "Timer.h"

static void periodic_timer_callback(void* arg);

// static esp_timer_create_args_t periodic_timer_args;

void Timer_phase_init(void)
{
    esp_timer_create_args_t periodic_timer_args = {
        .callback = &periodic_timer_callback,
        .name = "periodic"
    };

    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    // ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 500000));
}

void Timer_phase_cal()
{
    static uint16_t old_timer,new_timer;
}

static void periodic_timer_callback(void* arg)
{
    printf("hello");
}
