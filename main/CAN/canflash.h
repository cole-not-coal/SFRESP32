#ifndef CANflashH

#define CANflashH
#include "esp_partition.h"
#include "esp_ota_ops.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_timer.h"
#include "./../main.h"
#include "can.h"

esp_err_t CAN_flash_empty_queue(esp_partition_t *stOTAPartition);
dword CAN_flash_get_size();
esp_err_t CAN_flash_init();
word crc8(qword dwData);

extern dword dwBytesWrittenReflash;
extern word dwErrorCountReflash;
extern dword dwFirmwareSize;

#endif