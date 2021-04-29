#include <stdio.h>
#include "include/MQTT.h"
#include "driver/gpio.h"

#define BLINK_LED 2

void vTaskMQTTProcess(uint16_t topic_len,char *topic_send,uint16_t message_len ,char *messeage);

static const char *TAG = "MQTT_ESP32S2";

static const char *IP_Broker = "192.168.1.8";

esp_mqtt_client_handle_t client;

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
            ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            vTaskMQTTProcess(event->topic_len,event->topic,event->data_len,event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) 
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

void MQTT_init(void)
{
    esp_mqtt_client_config_t mqtt_cfg = 
    {
        .host = IP_Broker,
        .port = 1883,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
    vTaskMQTTSubscribe();
}

void vTaskMQTTPublish(float *input)
{
    char str_send[50];
    sprintf(str_send,"%f",*(input));
    esp_mqtt_client_publish(client,CONFIG_VOLTAGE_P_TOPIC,str_send,0,0,0);
    sprintf(str_send,"%f",*(input+1));
    esp_mqtt_client_publish(client,CONFIG_VOLTAGE_N_TOPIC,str_send,0,0,0);
    sprintf(str_send,"%f",*(input+2));
    esp_mqtt_client_publish(client,CONFIG_CURRENT_TOPIC,str_send,0,0,0);
}

void vTaskMQTTSubscribe(void)
{
    //dont do anything "lam thinh"
    esp_mqtt_client_subscribe(client,CONFIG_CONTROL_TOPIC,0);
    vTaskDelay(100);
}

void vTaskMQTTProcess(uint16_t topic_len,char *topic_input,uint16_t message_len ,char *message_input)
{
    // printf(topic_len);
    char *topic_sub=malloc(topic_len+1);
    char *message_sub=malloc(message_len+1);
    strncpy(topic_sub,topic_input,topic_len);
    strncpy(message_sub,message_input,message_len);
    topic_sub[topic_len]='\0';
    message_sub[message_len]='\0';
    #if DEBUG_MQTT
    printf("%d\n%d\n",topic_len,message_len);
    printf("%s\n",topic_sub);
    printf("%s\n",message_sub);
    if(strcmp(topic_sub,"client/control")==0)
    {
        printf("hi");
        if(strcmp(message_sub,"on")==0)
            gpio_set_level(BLINK_LED,1);
        else
            gpio_set_level(BLINK_LED,0);
    }
    #endif
    free(topic_sub);
    free(message_sub);
}