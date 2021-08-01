#include <stdio.h>
#include <stdbool.h>
#include "Timer.h"

static bool current_state_gpio=1;
float alpha_delay=0;

static void periodic_timer_callback(void* arg);

// static esp_timer_create_args_t periodic_timer_args;

void Timer_phase_init(void)
{
    // ESP_ERROR_CHECK(timer_set_divider(1,1,Clock_divider));

    esp_timer_create_args_t periodic_timer_args = {
        .callback = &periodic_timer_callback,
        .name = "Phase"
    };

    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &oneshot_timer));
}

void Timer_phase_cal(bool state)
{
    static uint64_t old_timer,new_timer=0;
    if(current_state_gpio==state)
    {
        return;
    }
    else
    {
        current_state_gpio=state;
    }
    if(state==0)
    {
        old_timer=esp_timer_get_time();
    }
    else if(state==1)
    {
        new_timer=esp_timer_get_time();
        float delta_timer=(new_timer-old_timer);
        if (delta_timer<0)
        {
            delta_timer=new_timer+(Max_timer-old_timer);
        }
        alpha_delay=(delta_timer/1000000)*Calib_phase;
        alpha_delay=alpha_delay*DEGtoRAD;
        
        #if DEBUG_USER_TIMER_PHASE
        printf("Phase delay: %f\n",alpha_delay);
        #endif
    }
}

void Timer_power_consumption(uint64_t *power_consumption,float *wattage)
{
    static uint64_t old_timer,new_timer=0;
    new_timer=esp_timer_get_time();
    float delta_timer=(new_timer-old_timer);
    if (delta_timer<0)
    {
        delta_timer=new_timer+(Max_timer-old_timer);
    }
    *power_consumption+=delta_timer*(*wattage);
    old_timer=esp_timer_get_time();

    #if DEBUG_USER_TIMER
    printf("Power consumption: %lld\n",*power_consumption/1000000);
    #endif
}

void Timer_Smartconfig(bool state)
{
    static uint64_t old_timer,new_timer=0;
    if(current_state_gpio==state)
    {
        return;
    }
    else
    {
        current_state_gpio=state;
    }
    if(state==0)
    {
        old_timer=esp_timer_get_time();
    }
    else if(state==1)
    {
        new_timer=esp_timer_get_time();
        float delta_timer=(new_timer-old_timer);
        if (delta_timer<0)
        {
            delta_timer=new_timer+(Max_timer-old_timer);
        }
        delta_timer/=1000000;
        if(delta_timer>1.8)
        {
            // vTaskSmartConfigStart();
        }
        
        #if DEBUG_USER_TIMER_SMART_CONFIG
        printf("Phase delay: %f\n",alpha_delay);
        #endif
    }
}

static void periodic_timer_callback(void* arg)
{
    printf("hello");
}
