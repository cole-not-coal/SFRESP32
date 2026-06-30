/*
tasks.c
File contains the tasks that run on the device. Tasks are called by interrupts at their specified rates.
Tasks are:
    task_BG: Background task that runs as often as processor time is available.
    task_1ms: Task that runs every 1ms.
    task_100ms: Task that runs every 100ms.

Written by Cole Perera and Aditya Parnandi for Sheffield Formula Racing 2025
*/
#include "tasks.h"

/* --------------------------- Local Types ----------------------------- */
stSensorMap_t stSensorMapTCell = {
    .fLowerLimit = 1.0f,
    .fUpperLimit = 3.0f,
    .afLookupTable = {
        {0, 1.304, 1.308, 1.311, 1.314, 1.317, 1.320, 1.322, 1.325, 1.328, 
        1.331, 1.334, 1.338, 1.341, 1.345, 1.350, 1.354, 1.359, 1.364, 1.369, 
        1.374, 1.380, 1.386, 1.392, 1.398, 1.405, 1.412, 1.419, 1.427, 1.435, 
        1.443, 1.451, 1.460, 1.469, 1.479, 1.489, 1.499, 1.510, 1.521, 1.533, 
        1.545, 1.557, 1.570, 1.584, 1.598, 1.613, 1.628, 1.643, 1.659, 1.676, 
        1.693, 1.710, 1.728, 1.746, 1.765, 1.784, 1.803, 1.823, 1.843, 1.863, 
        1.884, 1.904, 1.925, 1.946, 1.966, 1.987, 2.008, 2.029, 2.049, 2.070, 
        2.090, 2.110, 2.129, 2.148, 2.167, 2.185, 2.203, 2.220, 2.237, 2.253, 
        2.268, 2.283, 2.297, 2.310, 2.322, 2.334, 2.345, 2.355, 2.364, 2.373, 
        2.381, 2.389, 2.396, 2.402, 2.408, 2.414, 2.420, 2.426, 2.433, 5},
        {120.00, 118.38, 116.77, 115.15, 113.54, 111.92, 110.30, 108.69, 107.07, 105.45, 
        103.84, 102.22, 100.61, 98.99, 97.37, 95.76, 94.14, 92.53, 90.91, 89.29, 
        87.68, 86.06, 84.44, 82.83, 81.21, 79.60, 77.98, 76.36, 74.75, 73.13, 
        71.52, 69.90, 68.28, 66.67, 65.05, 63.43, 61.82, 60.20, 58.59, 56.97, 
        55.35, 53.74, 52.12, 50.51, 48.89, 47.27, 45.66, 44.04, 42.42, 40.81, 
        39.19, 37.58, 35.96, 34.34, 32.73, 31.11, 29.49, 27.88, 26.26, 24.65, 
        23.03, 21.41, 19.80, 18.18, 16.57, 14.95, 13.33, 11.72, 10.10, 08.48, 
        06.87, 05.25, 03.64, 02.02, 00.40, -1.21, -2.83, -4.44, -6.06, -7.68, 
        -9.29, -10.91, -12.53, -14.14, -15.76, -17.37, -18.99, -20.61, -22.22, -23.84, 
        -25.45, -27.07, -28.69, -30.30, -31.92, -33.54, -35.15, -36.77, -38.38, -40.00}       
    }
};


/* --------------------------- Local Variables ----------------------------- */ 
extern uint8_t byMACAddress[6];
extern esp_reset_reason_t eResetReason;
extern eChipMode_t eDeviceMode;
static esp_partition_t *stOTAPartition = NULL;
extern spi_device_handle_t MCP320XDevs[2];

/* --------------------------- Global Variables ----------------------------- */
dword adwMaxTaskTime[eTASK_TOTAL];
dword adwLastTaskTime[eTASK_TOTAL];
eTaskState_t astTaskState[eTASK_TOTAL];
dword dwTimeSincePowerUpms = 0;

/* --------------------------- Definitions ----------------------------- */
#define PERIOD_TASK_100MS 100   // ms
#define PERIOD_10S 10000        // ms
#define PERIOD_1S 1000          // ms
#define MAX_eREFLASH_TIME_US 300000000 // us

/* --------------------------- Functions ----------------------------- */
/* Background task that runs as often as processor time is available. */
void task_BG(void)
{ 
    static qword qwtTaskTimer;
    qwtTaskTimer = esp_timer_get_time();
    astTaskState[eTASK_BG] = eTASK_ACTIVE;

    /* Service the watchdog if all task have been completed at least once */
    word wNTaskCounter = 0;
    boolean bTasksComplete = TRUE;
    for (wNTaskCounter = 0; wNTaskCounter < eTASK_TOTAL; wNTaskCounter++)
    {
        if (astTaskState[wNTaskCounter] == eTASK_INACTIVE) {
            bTasksComplete = FALSE;
            break;
        }
    }
    if (bTasksComplete) {
        /* Reset the watchdog timer */
        (void)esp_task_wdt_reset();           
        
        for (wNTaskCounter = 0; wNTaskCounter < eTASK_TOTAL; wNTaskCounter++)
        {
            astTaskState[wNTaskCounter] = eTASK_INACTIVE;
        }
    }

    /* Update max task time */
    qwtTaskTimer = esp_timer_get_time() - qwtTaskTimer;
    adwLastTaskTime[eTASK_BG] = (dword)qwtTaskTimer;
    if (qwtTaskTimer > adwMaxTaskTime[eTASK_BG]) {
        adwMaxTaskTime[eTASK_BG] = (dword)qwtTaskTimer;
    }
}

/* Task that runs every 1ms. */
void task_1ms(void)
{
    qword qwtTaskTimer;
    static word NTempID = 0;
    float TCellRaw = 0.0f;
    int8_t TCellActual = 0;

    qwtTaskTimer = esp_timer_get_time();
    astTaskState[eTASK_1MS] = eTASK_ACTIVE;

    /* CAN error handling */
    CANRxCheck1ms();

    /* Update time since power up */
    dwTimeSincePowerUpms++;

    /* Read Temp Values */
    //Placeholder
    TCellRaw = MCP320X_read(MCP320XDevs[0], 0);
    TCellRaw = convert_sensor(TCellRaw, &stSensorMapTCell);
    if (TCellRaw < -40.0f || TCellRaw > 120.0f)
    {
        //Out of range error
        TCellActual = 127;
    } else
    {
        TCellActual = (int8_t)TCellRaw;
    }
    TCell[NTempID] = TCellActual;
    NTempID = (NTempID + 1) % 110;

    /* Update max task time */
    qwtTaskTimer = esp_timer_get_time() - qwtTaskTimer;
    adwLastTaskTime[eTASK_1MS] = (dword)qwtTaskTimer;
    if (qwtTaskTimer > adwMaxTaskTime[eTASK_1MS]) 
    {
        adwMaxTaskTime[eTASK_1MS] = (dword)qwtTaskTimer;
    }
}

/* Task that runs every 100ms. */
void task_100ms(void)
{
    
    static qword qwtTaskTimer;
    static word wNCounter;
    static word NTempID = 0;
    float TCellSum = 0.0f;
    NCellTemps = 110;
    qwtTaskTimer = esp_timer_get_time();
    astTaskState[eTASK_100MS] = eTASK_ACTIVE;

    /* CAN error handling */
    CANRxCheck1ms();

    TempMonAddressCastTx(stCANBus0);
    
    TCellMin = 127;
    TCellMax = -128;
    TCellSum = 0.0f;
    for (word wNCellCounter = 0; wNCellCounter < NCellTemps; wNCellCounter++)
    {
        if (TCell[wNCellCounter] < TCellMin)
        {
            TCellMin = TCell[wNCellCounter];
            NTCellMinID = wNCellCounter + 1;
        }
        if (TCell[wNCellCounter] > TCellMax)
        {
            TCellMax = TCell[wNCellCounter];
            NTCellMaxID = wNCellCounter + 1;
        }
        TCellSum += (float)TCell[wNCellCounter];
    }
    TCellAvg = (int8_t)(TCellSum / (float)NCellTemps);

    
    BMSCellTempTx(stCANBus0);

    for (word wNCellCounter = 0; wNCellCounter < 5; wNCellCounter++)
    {
        NTCellID = NTempID;
        CellTempGeneralTx(stCANBus0);
        NTempID = (NTempID + 1) % 110;
    }

    /* Every Second */
    if ( wNCounter % (PERIOD_1S / PERIOD_TASK_100MS) == 0 ) 
    {
        /* Toggle LED */
        pin_toggle(GPIO_ONBOARD_LED); 

        /* Send Status Message */
        /* This is not how you should send a CAN message but in this special case it is better this way */
        CAN_transmit(stCANBus0, &(CAN_frame_t)
        {
            .dwID = DEVICE_ID,
            .byDLC = 8,
            .abData = {
                (byte)(adwLastTaskTime[eTASK_1MS] / 50 & 0xFF),          
                (byte)(adwMaxTaskTime[eTASK_1MS] / 50 & 0xFF),
                (byte)(adwLastTaskTime[eTASK_100MS] / 500 & 0xFF),       
                (byte)(adwMaxTaskTime[eTASK_100MS] / 500 & 0xFF),
                (byte)(adwLastTaskTime[eTASK_BG] / 500 & 0xFF),        
                (byte)(adwMaxTaskTime[eTASK_BG] / 500 & 0xFF),
                (byte)((dwTimeSincePowerUpms/4000) >> 4 & 0xFF),
                (byte)(((dwTimeSincePowerUpms/4000) & 0xF) << 4 | (eResetReason & 0x0F)),
            }
        });
    };

    /* Every 10 Seconds */
    if (wNCounter >= PERIOD_10S / PERIOD_TASK_100MS)
    {
        /* Print the task times */
        #ifdef DEBUG
        ESP_LOGI(SFR_TAG, "Max Task Time: %5d BG %5d 1ms %5d 100ms", 
            (int)adwMaxTaskTime[eTASK_BG],
            (int)adwMaxTaskTime[eTASK_1MS],
            (int)adwMaxTaskTime[eTASK_100MS]);
        ESP_LOGI(SFR_TAG, "Last Task Time: %5d BG %5d 1ms %5d 100ms", 
            (int)adwLastTaskTime[eTASK_BG], 
            (int)adwLastTaskTime[eTASK_1MS],
            (int)adwLastTaskTime[eTASK_100MS]);
        wNCounter = 0;
        #endif
    }
    wNCounter++;

    /* Check if the CAN bus is in error state and recover */
    CAN_bus_diagnosics();

    /* Update max task time */
    qwtTaskTimer = esp_timer_get_time() - qwtTaskTimer;
    adwLastTaskTime[eTASK_100MS] = (dword)qwtTaskTimer;
    if (qwtTaskTimer > adwMaxTaskTime[eTASK_100MS]) 
    {
        adwMaxTaskTime[eTASK_100MS] = (dword)qwtTaskTimer;
    }
}

/* Task that runs every 1ms in reflash mode */
void reflash_task_1ms(void)
{

}

/* Task that runs every 100ms in reflash mode */
void reflash_task_100ms(void)
{
    pin_toggle(GPIO_ONBOARD_LED);
}

/* Background task for reflash mode */
void reflash_task_BG()
{
    static qword qwtReflashEntryTime = 0;
    esp_err_t eState;
    (void)esp_task_wdt_reset();

    /* Initialise Reflash Mode */
    if (qwtReflashEntryTime == 0)
    {
        ESP_LOGI("CANFLASH", "Entering reflash mode with firmware size %d bytes", dwFirmwareSize);
        qwtReflashEntryTime = (qword)esp_timer_get_time();
    }

    if (stOTAPartition == NULL)
    {
        stOTAPartition = (esp_partition_t *)esp_ota_get_next_update_partition(NULL);
        if (stOTAPartition == NULL)
        {
            ESP_LOGE("CANFLASH", "Failed to find OTA partition");
        } else 
        {
            ESP_LOGI("CANFLASH", "OTA partition found at address 0x%08X, size %d bytes",
                stOTAPartition->address, stOTAPartition->size);
            eState = esp_partition_erase_range(stOTAPartition, 0, stOTAPartition->size);
            if (eState != ESP_OK) {
                ESP_LOGE("CANFLASH", "Failed to erase OTA partition: %s", esp_err_to_name(eState));
                stOTAPartition = NULL;
            }
        }
    }

    /* Write new binary as its recieved */
    eState = CAN_flash_empty_queue(stOTAPartition);
    if (eState != ESP_OK)
    {
        ESP_LOGE("CANFLASH", "Failed to read reflash data to flash: %s", esp_err_to_name(eState));
    }
    eState = CAN_flash_write(stOTAPartition);
    if (eState != ESP_OK)
    {
        ESP_LOGE("CANFLASH", "Failed to write reflash data to flash: %s", esp_err_to_name(eState));
    }

    /* If binary fully received then restart */
    if (dwBytesWrittenReflash >= dwFirmwareSize && dwFirmwareSize > 0)
    {
        uint32_t dwFlashTime = (uint32_t)((esp_timer_get_time() - qwtReflashEntryTime)/1000000);
        ESP_LOGI("CANFLASH", "Reflash complete, written %d bytes in %d s", (int)dwBytesWrittenReflash, dwFlashTime);
        eState = esp_ota_set_boot_partition(stOTAPartition);
        if (eState == ESP_OK) {
            esp_restart();
        }
        ESP_LOGE("CANFLASH", "Failed to set boot partition: %s", esp_err_to_name(eState));
    }
}

void pin_toggle(gpio_num_t ePin)
{
    static boolean BLEDState = false;
    BLEDState = !BLEDState;
    gpio_set_level(ePin, BLEDState);
}
