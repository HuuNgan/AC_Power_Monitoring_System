#ifndef FLASH_H
#define FLASH_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_flash.h"
#include "esp_flash_spi_init.h"

#define WIFI_SSID_FLASH_LOC     0x002FE000
#define WIFI_PSWD_FLASH_LOC     0x002FD000

char SSID[32];
char PSWD[32];

void SPI_Init(void);
void Flash_read(void);
void Flash_write(void);

#endif
