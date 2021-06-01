#include <stdio.h>
#include "SPI_Flash.h"

static uint32_t Wifi_SSID_loc=WIFI_SSID_FLASH_LOC;
static uint32_t Wifi_PSWD_loc=WIFI_PSWD_FLASH_LOC;

void SPI_Init(void)
{
    esp_flash_init(esp_flash_default_chip);
}

void Flash_read(void)
{
    esp_flash_write(NULL,SSID,Wifi_SSID_loc,32);
    esp_flash_write(NULL,PSWD,Wifi_PSWD_loc,32);
}

void Flash_write(void)
{
    esp_flash_read(NULL,SSID,Wifi_SSID_loc,32);
    esp_flash_read(NULL,PSWD,Wifi_PSWD_loc,32);
}