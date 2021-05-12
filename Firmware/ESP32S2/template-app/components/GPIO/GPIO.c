#include <stdio.h>
#include "GPIO.h"

static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR GPIO_ISR_Handler(void* arg)
{
    //do thing
    uint32_t gpio_num = (uint32_t) arg;
    // xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void vTaskGPIO_ISR_Process(void* arg)
{
    uint32_t io_num;
    for(;;)
    {
        //do thing
        printf("hi\n");
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) 
        {
            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
    }
}

void GPIO_init(void)
{
    gpio_config_t gpio_init_cfg;

    //config built in LED
    gpio_init_cfg.pin_bit_mask=BLINK_LED_PIN_SEL;
    gpio_init_cfg.mode=GPIO_MODE_OUTPUT;
    gpio_init_cfg.intr_type=GPIO_INTR_DISABLE;
    gpio_init_cfg.pull_down_en=0;
    gpio_init_cfg.pull_up_en=0;
    gpio_config(&gpio_init_cfg);

    //config button
    gpio_init_cfg.pin_bit_mask=BUTTON_PIN_SEL;
    gpio_init_cfg.mode=GPIO_MODE_INPUT;
    gpio_init_cfg.intr_type=GPIO_INTR_ANYEDGE;
    gpio_init_cfg.pull_down_en=1;
    gpio_init_cfg.pull_up_en=0;
    gpio_config(&gpio_init_cfg);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(vTaskGPIO_ISR_Process,"GPIO_Task_process",512,NULL,5,NULL);
    gpio_isr_handler_add(BUTTON, GPIO_ISR_Handler, (void*) BUTTON);
}

void control_other_pin(uint16_t pin, bool control)
{
    //do thing
}

void control_built_in_led(bool control)
{
    gpio_set_level(BLINK_LED,control);
}
