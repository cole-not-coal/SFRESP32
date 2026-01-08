/*
tasks.c
File contains the tasks that run on the device. Tasks are called by interrupts at their specified rates.
Tasks are:
    task_BG: Background task that runs as often as processor time is available.
    task_1ms: Task that runs every 1ms.
    task_10ms: Task that runs every 10ms.
    task_100ms: Task that runs every 100ms.

Written by Cole Perera and Aditya Parnandi for Sheffield Formula Racing 2025
*/
#include "tasks.h"

/* --------------------------- Local Types ----------------------------- */


/* --------------------------- Local Variables ----------------------------- */ 
extern uint8_t byMACAddress[6];
extern esp_reset_reason_t eResetReason;
extern eChipMode_t eDeviceMode;
static esp_partition_t *stOTAPartition = NULL;
extern stADCHandles_t stADCHandle1;
extern stADCHandles_t stADCHandle2;
/* --------------------------- Definitions ----------------------------- */
#define TIME_BETWEEN_CAN_MSGS 100  // ms

/* Contactor IDs */
#define PRECHARGE_CONTACTOR_ID 1          // Precharge contactor (positive terminal only)
#define MAIN_CONTACTOR_POSITIVE_ID 2      // Main contactor on positive terminal
#define MAIN_CONTACTOR_NEGATIVE_ID 3      // Main contactor on negative terminal

/* voltage thresholds for contactor state verification */
#define VOLTAGE_THRESHOLD_PRECHARGE 5.0   // voltage difference threshold for precharge complete (V)
#define VOLTAGE_THRESHOLD_MAIN 2.0        // voltage difference threshold for main contactor closed (V)

/* --------------------------- Global Variables ----------------------------- */
dword adwMaxTaskTime[eTASK_TOTAL];
dword adwLastTaskTime[eTASK_TOTAL];
eTaskState_t astTaskState[eTASK_TOTAL];
dword dwTimeSincePowerUpms = 0;

/* --------------------------- Local Variables ----------------------------- */
static boolean bBMSOK = FALSE;                      // BMS health status (set by external function)
static boolean bStartupRequested = FALSE;           // Startup sequence requested
static boolean bPrechargeComplete = FALSE;          // Precharge complete flag
static boolean bMainContactorsClosed = FALSE;       // Main contactors closed flag (startup complete)

/* --------------------------- Definitions ----------------------------- */
#define PERIOD_TASK_100MS 100   // ms
#define PERIOD_10S 10000        // ms
#define PERIOD_1S 1000          // ms
#define MAX_eREFLASH_TIME_US 300000000 // us

/* --------------------------- Function prototypes ----------------------------- */
void pin_toggle(gpio_num_t pin);
void task_BG(void);
void task_1ms(void);
void task_100ms(void);
void reflash_task_100ms(void);
void check_main_contactors_closed(void);
void handle_contactor_sequence(void);
void monitor_BMS_state(void);

/* --------------------------- Functions ----------------------------- */
void task_BG(void)
{
    /* Background task that runs as often as processor time is available. */
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

void task_1ms(void)
{
    /* Task that runs every 1ms. */
    qword qwtTaskTimer;
    qwtTaskTimer = esp_timer_get_time();
    astTaskState[eTASK_1MS] = eTASK_ACTIVE;

    /* check BMS ON */
    //BMS ok then 
        //if contactor closed then precharge
        //if precharged close main contactor
        // contactor closed then do nothing
    //BMS not ok then open main contactor

    /* Update contactor status flags by checking voltages */
    /*
    * Check if precharge is complete by comparing voltages across precharge contactors
    * Sets bPrechargeComplete flag to TRUE when voltage difference is within threshold
    */
    
    float fVPrechargeAccuSide = 0.0;   // voltage on accumulator side (battery side)
    float fVPrechargeCarSide = 0.0;    // voltage on car side (load side)
    
    // Read ADC values
    fVPrechargeAccuSide = adc_read_voltage(&stADCHandle1);
    fVPrechargeCarSide = adc_read_voltage(&stADCHandle2);

    float fVDifference = fabsf(fVPrechargeCarSide - fVPrechargeAccuSide);
    if (fVDifference < VOLTAGE_THRESHOLD_PRECHARGE) {
        bPrechargeComplete = TRUE;
        ESP_LOGI(SFR_TAG, "Precharge complete: voltage diff = %.2fV", fVDifference);
    } else {
        bPrechargeComplete = FALSE;
    }

    check_main_contactors_closed(); //reads ADCs and updates bMainContactorsClosed
    
    /* Handle contactor sequence based on flags */
    handle_contactor_sequence();
    
    /* Always monitor BMS state (background safety check) */
    monitor_BMS_state();

    /* Update time since power up */
    dwTimeSincePowerUpms++;

    /* Update max task time */
    qwtTaskTimer = esp_timer_get_time() - qwtTaskTimer;
    adwLastTaskTime[eTASK_1MS] = (dword)qwtTaskTimer;
    if (qwtTaskTimer > adwMaxTaskTime[eTASK_1MS]) 
    {
        adwMaxTaskTime[eTASK_1MS] = (dword)qwtTaskTimer;
    }
}

void task_100ms(void)
{
    /* Task that runs every 100ms. */
    static qword qwtTaskTimer;
    static word wNCounter;

    qwtTaskTimer = esp_timer_get_time();
    astTaskState[eTASK_100MS] = eTASK_ACTIVE;

    /* Every Second */
    if ( wNCounter % (PERIOD_1S / PERIOD_TASK_100MS) == 0 ) 
    {
        /* Toggle LED */
        pin_toggle(GPIO_ONBOARD_LED); 

        /* Send Status Message */
        CAN_transmit(stCANBus0, &(CAN_frame_t)
        {
            .dwID = DEVICE_ID, // UPDATE THIS FOR EACH DEVICE
            .byDLC = 8,
            .abData = {
                (byte)(adwLastTaskTime[eTASK_1MS] / 50 & 0xFF),          
                (byte)(adwMaxTaskTime[eTASK_1MS] / 50 & 0xFF),
                (byte)(adwLastTaskTime[eTASK_100MS] / 500 & 0xFF),       
                (byte)(adwMaxTaskTime[eTASK_100MS] / 500 & 0xFF),
                (byte)(adwLastTaskTime[eTASK_BG] / 500 & 0xFF),        
                (byte)(adwMaxTaskTime[eTASK_BG] / 500 & 0xFF),
                (byte)((dwTimeSincePowerUpms/4000) & 0xFF),
                (byte)(((dwTimeSincePowerUpms/4000) >> 8 & 0xF) | (eResetReason & 0x0F << 4)),
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

void reflash_task_1ms(void)
{

}

void reflash_task_100ms(void)
{
    pin_toggle(GPIO_ONBOARD_LED);
}

void reflash_task_BG()
{
    static qword qwtReflashEntryTime = 0;
    esp_err_t eState;

    (void)esp_task_wdt_reset();

    /* Initialise Reflash Mode */
    if (qwtReflashEntryTime == 0)
    {
        dwFirmwareSize = 0; //Reset firmware size
        CAN_flash_get_size();
        CAN_clear_rx_buffer();
        qwtReflashEntryTime = (qword)esp_timer_get_time();
    }

    if (stOTAPartition == NULL)
    {
        stOTAPartition = esp_ota_get_next_update_partition(NULL);
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

void check_main_contactors_closed(void)
{
    /*
    * Check if main positive contactor is closed by comparing voltages
    * Main negative is closed during precharge, so only need to check positive
    * Sets bMainContactorsClosed flag to TRUE when voltage difference is within threshold
    */
    
    float fMainPosAccuSide = 0.0;    // voltage on accumulator side (battery +)
    float fMainPosCarSide = 0.0;     // voltage on car side (load +)

    // Read ADC values for main positive contactor - reusing same ADC handles
    fMainPosAccuSide = adc_read_voltage(&stADCHandle1);
    fMainPosCarSide = adc_read_voltage(&stADCHandle2);
    
    float fVDifference = fabsf(fMainPosAccuSide - fMainPosCarSide);
    
    if (fVDifference < VOLTAGE_THRESHOLD_MAIN) {
        bMainContactorsClosed = TRUE;
        ESP_LOGI(SFR_TAG, "Main positive contactor closed: voltage diff = %.2fV", fVDifference);
    } else {
        bMainContactorsClosed = FALSE;
    }
}

void handle_contactor_sequence(void)
{
    /*
    * Simple sequential contactor control based on flags
    * Commands are sent immediately, flags are updated by check functions
    * No blocking - just send commands and continue execution
    * 
    * Sequence:
    * 1. Close negative main contactor + precharge contactor (precharge phase)
    * 2. Wait for precharge complete (voltages equalize)
    * 3. Close positive main contactor (startup complete)
    */
    
    // Step 1: If startup requested and BMS OK, close negative main + precharge
    if (bStartupRequested && bBMSOK && !bPrechargeComplete) {
        ESP_LOGI(SFR_TAG, "Startup requested - Closing negative main contactor and precharge contactor");
        turn_on_contactor(MAIN_CONTACTOR_NEGATIVE_ID, GPIO_MAIN_NEG_CONTACTOR_PWM);
        turn_on_contactor(PRECHARGE_CONTACTOR_ID, GPIO_PRECHARGE_CONTACTOR_PWM);
        // Don't return - let code continue, flags will update in check functions
    }
    
    // Step 2: If precharge complete, close main positive contactor
    if (bPrechargeComplete && !bMainContactorsClosed) {
        ESP_LOGI(SFR_TAG, "Precharge complete - Closing main positive contactor");
        turn_on_contactor(MAIN_CONTACTOR_POSITIVE_ID, GPIO_MAIN_POS_CONTACTOR_PWM);
        // Don't return - let code continue, flags will update in check functions
    }
    
    // Step 3: If main positive closed, optionally open precharge contactor
    if (bMainContactorsClosed && bPrechargeComplete) {
        // TODO: Decide if you want to open precharge contactor after main positive closes
        // This depends on your circuit design - some keep it closed, some open it
        // turn_off_contactor(PRECHARGE_CONTACTOR_ID, GPIO_PRECHARGE_CONTACTOR_PWM);
        
        ESP_LOGI(SFR_TAG, "Startup complete - System operational");
    }
    
    // Execution continues - rest of code runs normally
}

void monitor_BMS_state(void)
{
    /*
    * Continuous background monitoring of BMS state
    * If BMS goes NOT OK, immediately open all contactors for safety
    * bBMSOK flag is assumed to be set by external function
    */
    
    if (!bBMSOK) {
        // BMS fault detected - emergency shutdown
        if (bMainContactorsClosed || bPrechargeComplete) {
            ESP_LOGE(SFR_TAG, "BMS FAULT - Emergency shutdown - Opening all contactors");
            
            // Open all contactors immediately
            turn_off_contactor(MAIN_CONTACTOR_POSITIVE_ID, GPIO_MAIN_POS_CONTACTOR_PWM);
            turn_off_contactor(MAIN_CONTACTOR_NEGATIVE_ID, GPIO_MAIN_NEG_CONTACTOR_PWM);
            turn_off_contactor(PRECHARGE_CONTACTOR_ID, GPIO_PRECHARGE_CONTACTOR_PWM);

            // Reset all flags
            bStartupRequested = FALSE;
            bPrechargeComplete = FALSE;
            bMainContactorsClosed = FALSE;
        }
    }
    // If BMS is OK and contactors are closed, do nothing - keep them closed
}

