#ifndef SFRTasks
#include "driver/gpio.h"
#include <stdint.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "esp_timer.h"
#include "rom/ets_sys.h"
#include "sfrtypes.h"

#include "pin.h"
#include "CAN/can.h"
#include "espnow.h"
#include "sdcard.h"
#include "contactors.h"
#include "adc.h"
#include "I2C.h"
#include "NVHDisplay.h"
#include "CAN/canflash.h"
#include "main.h"
#include "CAN/canDecodeAuto.h"
#include "mcp320X.h"

#define CELL_COUNT 88 //Number of cells to monitor

/* --------------------------- Function prototypes ----------------------------- */
void task_BG(void);
void task_1ms(void);
void task_100ms(void);
void reflash_task_100ms(void);
void pin_toggle(gpio_num_t pin);
void reflash_task_BG();
float read_cell(uint8_t byNCell);
void input_select(uint8_t byNInput);

#define SFRTasks
#endif