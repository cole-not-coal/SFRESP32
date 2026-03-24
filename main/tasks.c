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
extern stADCHandles_t stADCHandle0;
extern stADCHandles_t stADCHandle1;
extern stADCHandles_t stADCHandle2;
extern stADCHandles_t stADCHandle3;
extern stADCHandles_t stADCHandle4;
extern stADCHandles_t stADCHandle5;
extern stADCHandles_t stADCHandle6;
float fVDynoPressure1Raw = 0.0;
float fVDynoPressure2Raw = 0.0;
float fVDynoPressure3Raw = 0.0;
float fVDynoTemp1Raw = 0.0;
float fVDynoTemp2Raw = 0.0;
float fVDynoTemp3Raw = 0.0;
float fVDynoFlowRaw = 0.0;
float fpDynoPressure1 = 0.0;
float fpDynoPressure2 = 0.0;
float fpDynoPressure3 = 0.0;
float fTDynoTemp1 = 0.0;
float fTDynoTemp2 = 0.0;
float fTDynoTemp3 = 0.0;
float fVDynoFlow = 0.0; // L/min

stSensorMap_t stDynoTempMap = {
    .fLowerLimit = 0.0f,
    .fUpperLimit = 5.0f,
    .afLookupTable = {
        {0.00f, 0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f, 0.35f, 0.40f, 0.45f, 
        0.50f, 0.55f, 0.60f, 0.65f, 0.70f, 0.75f, 0.80f, 0.85f, 0.90f, 0.95f, 
        1.00f, 1.05f, 1.10f, 1.15f, 1.20f, 1.25f, 1.30f, 1.35f, 1.40f, 1.45f, 
        1.50f, 1.55f, 1.60f, 1.65f, 1.70f, 1.75f, 1.80f, 1.85f, 1.90f, 1.95f, 
        2.00f, 2.05f, 2.10f, 2.15f, 2.20f, 2.25f, 2.30f, 2.35f, 2.40f, 2.45f, 
        2.50f, 2.55f, 2.60f, 2.65f, 2.70f, 2.75f, 2.80f, 2.85f, 2.90f, 2.95f, 
        3.00f, 3.05f, 3.10f, 3.15f, 3.20f, 3.25f, 3.30f, 3.35f, 3.40f, 3.45f, 
        3.50f, 3.55f, 3.60f, 3.65f, 3.70f, 3.75f, 3.80f, 3.85f, 3.90f, 3.95f, 
        4.00f, 4.05f, 4.10f, 4.15f, 4.20f, 4.25f, 4.30f, 4.35f, 4.40f, 4.45f, 
        4.50f, 4.55f, 4.60f, 4.65f, 4.70f, 4.75f, 4.80f, 4.85f, 4.90f, 4.95f, 5.00f},
        {140.00f, 140.00f, 127.43f, 109.80f,  98.39f,  89.58f,  82.95f,  77.28f,  72.35f,  68.12f, 
        64.51f,    60.82f,  57.96f,  55.29f,  52.56f,  49.84f,  47.92f,  45.96f,  43.94f,  41.88f, 
        39.84f,    38.40f,  36.93f,  35.41f,  33.85f,  32.25f,  30.61f,  29.31f,  28.18f,  27.03f, 
        25.84f,    24.62f,  23.37f,  22.07f,  20.74f,  19.60f,  18.70f,  17.78f,  16.82f,  15.83f, 
        14.81f,    13.76f,  12.67f,  11.54f,  10.37f,   9.48f,   8.71f,   7.91f,   7.07f,   6.21f, 
        5.30f,      4.36f,   3.39f,   2.37f,   1.30f,   0.19f,  -0.58f,  -1.31f,  -2.07f,  -2.87f, 
        -3.71f,    -4.59f,  -5.52f,  -6.50f,  -7.54f,  -8.63f,  -9.79f, -10.59f, -11.35f, -12.16f, 
        -13.02f,  -13.94f, -14.92f, -15.98f, -17.12f, -18.35f, -19.69f, -20.64f, -21.53f, -22.50f, 
        -23.57f,  -24.75f, -26.07f, -27.54f, -29.19f, -30.57f, -31.73f, -33.07f, -34.62f, -36.46f, 
        -38.67f,  -40.00f, -40.00f, -40.00f, -40.00f, -40.00f, -40.00f, -40.00f, -40.00f, -40.00f, -40.00f}},
};

stSensorMap_t stDynoPressureMap = {
    .fLowerLimit = 0.0f,
    .fUpperLimit = 5.0f,
    .afLookupTable = {
        {0.00f, 0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f, 0.35f, 0.40f, 0.45f, 
        0.50f, 0.55f, 0.60f, 0.65f, 0.70f, 0.75f, 0.80f, 0.85f, 0.90f, 0.95f, 
        1.00f, 1.05f, 1.10f, 1.15f, 1.20f, 1.25f, 1.30f, 1.35f, 1.40f, 1.45f, 
        1.50f, 1.55f, 1.60f, 1.65f, 1.70f, 1.75f, 1.80f, 1.85f, 1.90f, 1.95f, 
        2.00f, 2.05f, 2.10f, 2.15f, 2.20f, 2.25f, 2.30f, 2.35f, 2.40f, 2.45f, 
        2.50f, 2.55f, 2.60f, 2.65f, 2.70f, 2.75f, 2.80f, 2.85f, 2.90f, 2.95f, 
        3.00f, 3.05f, 3.10f, 3.15f, 3.20f, 3.25f, 3.30f, 3.35f, 3.40f, 3.45f, 
        3.50f, 3.55f, 3.60f, 3.65f, 3.70f, 3.75f, 3.80f, 3.85f, 3.90f, 3.95f, 
        4.00f, 4.05f, 4.10f, 4.15f, 4.20f, 4.25f, 4.30f, 4.35f, 4.40f, 4.45f, 
        4.50f, 4.55f, 4.60f, 4.65f, 4.70f, 4.75f, 4.80f, 4.85f, 4.90f, 4.95f, 5.00f},
        {-1.00f, -0.90f, -0.80f, -0.70f, -0.60f, -0.50f, -0.40f, -0.30f, -0.20f, -0.10f, 
        0.00f, 0.10f, 0.20f, 0.30f, 0.40f, 0.50f, 0.60f, 0.70f, 0.80f, 0.90f, 
        1.00f, 1.10f, 1.20f, 1.30f, 1.40f, 1.50f, 1.60f, 1.70f, 1.80f, 1.90f, 
        2.00f, 2.10f, 2.20f, 2.30f, 2.40f, 2.50f, 2.60f, 2.70f, 2.80f, 2.90f, 
        3.00f, 3.10f, 3.20f, 3.30f, 3.40f, 3.50f, 3.60f, 3.70f, 3.80f, 3.90f, 
        4.00f, 4.10f, 4.20f, 4.30f, 4.40f, 4.50f, 4.60f, 4.70f, 4.80f, 4.90f, 
        5.00f, 5.10f, 5.20f, 5.30f, 5.40f, 5.50f, 5.60f, 5.70f, 5.80f, 5.90f, 
        6.00f, 6.10f, 6.20f, 6.30f, 6.40f, 6.50f, 6.60f, 6.70f, 6.80f, 6.90f, 
        7.00f, 7.10f, 7.20f, 7.30f, 7.40f, 7.50f, 7.60f, 7.70f, 7.80f, 7.90f, 
        8.00f, 8.10f, 8.20f, 8.30f, 8.40f, 8.50f, 8.60f, 8.70f, 8.80f, 8.90f, 9.00f}},
};

stSensorMap_t stDynoFlowMap = {
    .fLowerLimit = 0.0f,
    .fUpperLimit = 5.0f,
    .afLookupTable = {
        {0.00f, 0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f, 0.35f, 0.40f, 0.45f, 
        0.50f, 0.55f, 0.60f, 0.65f, 0.70f, 0.75f, 0.80f, 0.85f, 0.90f, 0.95f, 
        1.00f, 1.05f, 1.10f, 1.15f, 1.20f, 1.25f, 1.30f, 1.35f, 1.40f, 1.45f, 
        1.50f, 1.55f, 1.60f, 1.65f, 1.70f, 1.75f, 1.80f, 1.85f, 1.90f, 1.95f, 
        2.00f, 2.05f, 2.10f, 2.15f, 2.20f, 2.25f, 2.30f, 2.35f, 2.40f, 2.45f, 
        2.50f, 2.55f, 2.60f, 2.65f, 2.70f, 2.75f, 2.80f, 2.85f, 2.90f, 2.95f, 
        3.00f, 3.05f, 3.10f, 3.15f, 3.20f, 3.25f, 3.30f, 3.35f, 3.40f, 3.45f, 
        3.50f, 3.55f, 3.60f, 3.65f, 3.70f, 3.75f, 3.80f, 3.85f, 3.90f, 3.95f, 
        4.00f, 4.05f, 4.10f, 4.15f, 4.20f, 4.25f, 4.30f, 4.35f, 4.40f, 4.45f, 
        4.50f, 4.55f, 4.60f, 4.65f, 4.70f, 4.75f, 4.80f, 4.85f, 4.90f, 4.95f, 5.00f},
        {0.0000f, 0.0024f, 0.0048f, 0.0073f, 0.0097f, 0.0121f, 0.0145f, 0.0169f, 0.0194f, 0.0218f, 
        0.0242f, 0.0266f, 0.0290f, 0.0315f, 0.0339f, 0.0363f, 0.0387f, 0.0411f, 0.0435f, 0.0460f, 
        0.0484f, 0.0508f, 0.0532f, 0.0556f, 0.0581f, 0.0605f, 0.0629f, 0.0653f, 0.0677f, 0.0702f, 
        0.0726f, 0.0750f, 0.0774f, 0.0798f, 0.0823f, 0.0847f, 0.0871f, 0.0895f, 0.0919f, 0.0944f, 
        0.0968f, 0.0992f, 0.1016f, 0.1040f, 0.1065f, 0.1089f, 0.1113f, 0.1137f, 0.1161f, 0.1185f, 
        0.1210f, 0.1234f, 0.1258f, 0.1282f, 0.1306f, 0.1331f, 0.1355f, 0.1379f, 0.1403f, 0.1427f, 
        0.1452f, 0.1476f, 0.1500f, 0.1524f, 0.1548f, 0.1573f, 0.1597f, 0.1621f, 0.1645f, 0.1669f, 
        0.1694f, 0.1718f, 0.1742f, 0.1766f, 0.1790f, 0.1815f, 0.1839f, 0.1863f, 0.1887f, 0.1911f, 
        0.1935f, 0.1960f, 0.1984f, 0.2008f, 0.2032f, 0.2056f, 0.2081f, 0.2105f, 0.2129f, 0.2153f, 
        0.2177f, 0.2202f, 0.2226f, 0.2250f, 0.2274f, 0.2298f, 0.2323f, 0.2347f, 0.2371f, 0.2395f, 0.24f}},
};

/* --------------------------- Global Variables ----------------------------- */
dword adwMaxTaskTime[eTASK_TOTAL];
dword adwLastTaskTime[eTASK_TOTAL];
eTaskState_t astTaskState[eTASK_TOTAL];
dword dwTimeSincePowerUpms = 0;

/* --------------------------- Function prototypes ----------------------------- */

/* --------------------------- Definitions ----------------------------- */
#define PERIOD_TASK_100MS 100   // ms
#define PERIOD_10S 10000        // ms
#define PERIOD_1S 1000          // ms
#define MAX_eREFLASH_TIME_US 300000000 // us
#define DYNO_PRESSURE_CAN_GAIN 2000
#define DYNO_PRESSURE_CAN_OFFSET 2000
#define DYNO_TEMP_CAN_GAIN 200
#define DYNO_TEMP_CAN_OFFSET 10000
#define DYNO_FLOW_CAN_GAIN 100000
#define DYNO_PRESSURE_CAN_GAIN_RAW 10000
#define DYNO_TEMP_CAN_GAIN_RAW 10000
#define DYNO_FLOW_CAN_GAIN_RAW 10000

/* --------------------------- Function prototypes ----------------------------- */
void pin_toggle(gpio_num_t pin);
void task_BG(void);
void task_1ms(void);
void task_100ms(void);
void reflash_task_100ms(void);

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

    // fVDynoPressure1Raw = adc_read_voltage(&stADCHandle0);
    // fVDynoPressure2Raw = adc_read_voltage(&stADCHandle1);
    // fVDynoPressure3Raw = adc_read_voltage(&stADCHandle2);
    fVDynoTemp1Raw = adc_read_voltage(&stADCHandle3);
    // fVDynoTemp2Raw = adc_read_voltage(&stADCHandle4);
    fVDynoTemp3Raw = adc_read_voltage(&stADCHandle5);
    fVDynoFlowRaw = adc_read_voltage(&stADCHandle6);

    // fpDynoPressure1 = read_sensor(&stADCHandle0, &stDynoPressureMap);
    // fpDynoPressure2 = read_sensor(&stADCHandle1, &stDynoPressureMap);
    // fpDynoPressure3 = read_sensor(&stADCHandle2, &stDynoPressureMap);
    fTDynoTemp1 = read_sensor(&stADCHandle3, &stDynoTempMap);
    // fTDynoTemp2 = read_sensor(&stADCHandle4, &stDynoTempMap);
    fTDynoTemp3 = read_sensor(&stADCHandle5, &stDynoTempMap);
    fVDynoFlow = read_sensor(&stADCHandle6, &stDynoFlowMap);

    /* Transmit Raw Volatages */
    CAN_frame_t stTxFrame = {
        .dwID = 0x90,
        .byDLC = 8
    };
    word wTemp = (word)(fVDynoPressure1Raw * DYNO_PRESSURE_CAN_GAIN_RAW);
    stTxFrame.abData[0] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[1] = (byte)((wTemp >> 8) & 0xFF);
    wTemp = (word)(fVDynoPressure2Raw * DYNO_PRESSURE_CAN_GAIN_RAW);
    stTxFrame.abData[2] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[3] = (byte)((wTemp >> 8) & 0xFF);
    wTemp = (word)(fVDynoPressure3Raw * DYNO_PRESSURE_CAN_GAIN_RAW);
    stTxFrame.abData[4] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[5] = (byte)((wTemp >> 8) & 0xFF);
    wTemp = (word)(fVDynoFlow * DYNO_FLOW_CAN_GAIN_RAW);
    stTxFrame.abData[6] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[7] = (byte)((wTemp >> 8) & 0xFF);
    CAN_transmit(stCANBus0, &stTxFrame);

    stTxFrame.dwID = 0x91;
    stTxFrame.byDLC = 6;
    wTemp = (word)(fVDynoTemp1Raw * DYNO_TEMP_CAN_GAIN_RAW);
    stTxFrame.abData[0] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[1] = (byte)((wTemp >> 8) & 0xFF);
    wTemp = (word)(fVDynoTemp2Raw * DYNO_TEMP_CAN_GAIN_RAW);
    stTxFrame.abData[2] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[3] = (byte)((wTemp >> 8) & 0xFF);
    wTemp = (word)(fVDynoTemp3Raw * DYNO_TEMP_CAN_GAIN_RAW);
    stTxFrame.abData[4] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[5] = (byte)((wTemp >> 8) & 0xFF);
    CAN_transmit(stCANBus0, &stTxFrame);

    /* Transmit Processed Sensor Values */
    stTxFrame.dwID = 0x92;
    stTxFrame.byDLC = 8;
    wTemp = (word)(fpDynoPressure1 * DYNO_PRESSURE_CAN_GAIN) + DYNO_PRESSURE_CAN_OFFSET;
    stTxFrame.abData[0] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[1] = (byte)((wTemp >> 8) & 0xFF);
    wTemp = (word)(fpDynoPressure2 * DYNO_PRESSURE_CAN_GAIN) + DYNO_PRESSURE_CAN_OFFSET;
    stTxFrame.abData[2] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[3] = (byte)((wTemp >> 8) & 0xFF);
    wTemp = (word)(fpDynoPressure3 * DYNO_PRESSURE_CAN_GAIN) + DYNO_PRESSURE_CAN_OFFSET;
    stTxFrame.abData[4] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[5] = (byte)((wTemp >> 8) & 0xFF);
    wTemp = (word)(fVDynoFlow * DYNO_FLOW_CAN_GAIN);
    stTxFrame.abData[6] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[7] = (byte)((wTemp >> 8) & 0xFF);
    CAN_transmit(stCANBus0, &stTxFrame);

    stTxFrame.dwID = 0x93;
    stTxFrame.byDLC = 6;
    wTemp = (word)(fTDynoTemp1 * DYNO_TEMP_CAN_GAIN) + DYNO_TEMP_CAN_OFFSET;
    stTxFrame.abData[0] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[1] = (byte)((wTemp >> 8) & 0xFF);
    wTemp = (word)(fTDynoTemp2 * DYNO_TEMP_CAN_GAIN) + DYNO_TEMP_CAN_OFFSET;
    stTxFrame.abData[2] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[3] = (byte)((wTemp >> 8) & 0xFF);
    wTemp = (word)(fTDynoTemp3 * DYNO_TEMP_CAN_GAIN) + DYNO_TEMP_CAN_OFFSET;
    stTxFrame.abData[4] = (byte)(wTemp & 0xFF);
    stTxFrame.abData[5] = (byte)((wTemp >> 8) & 0xFF);
    CAN_transmit(stCANBus0, &stTxFrame);


    /* Every Second */
    if ( wNCounter % (PERIOD_1S / PERIOD_TASK_100MS) == 0 ) 
    {
        /* Toggle LED */
        pin_toggle(GPIO_ONBOARD_LED); 

        /* Send Status Message */
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
       
        ESP_LOGI("ADC", "Pressures (Bar): %3.2f | %3.2f | %3.2f", fpDynoPressure1, fpDynoPressure2, fpDynoPressure3);
        ESP_LOGI("ADC", "Temperatures (degC): %3.2f | %3.2f | %3.2f", fTDynoTemp1, fTDynoTemp2, fTDynoTemp3);
        ESP_LOGI("ADC", "Flow Rate (L/min): %3.2f", fVDynoFlow);
 
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
