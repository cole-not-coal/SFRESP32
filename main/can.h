#ifndef SFRCAN
#include "main.h"

#include "esp_twai.h"
#include "esp_twai_onchip.h"

#include "pin.h"
#include "string.h"
#include "espnow.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define CAN_CMD_ID 0x010
typedef enum {
    eCMD_RESET          = 0x1,
    eCMD_CLEAR_MINMAX   = 0x2,
    eCMD_CLEAR_ERRORS   = 0x3,
    eCMD_REFLASH_MODE   = 0x4,
    eCMD_NORMAL_MODE    = 0x5,
} eCAN_CMD_t;

esp_err_t CAN_init(boolean bEnableRx);
esp_err_t CAN_transmit(twai_node_handle_t stCANBus, const CAN_frame_t *stFrame);
bool CAN_receive_callback(twai_node_handle_t stCANBus, const twai_rx_done_event_data_t *edata, void *stRxCallback);
esp_err_t CAN_receive_debug();
void CAN_bus_diagnosics();
const char* CAN_error_state_to_string(twai_error_state_t stState);
esp_err_t CAN_empty_ESPNOW_buffer(twai_node_handle_t stCANBus);
bool CAN_receive_callback_no_queue(twai_node_handle_t stCANBus, const twai_rx_done_event_data_t *edata, void *stRxCallback);
void CAN_CMD_response(twai_frame_t stRxFrame);
void CAN_clear_rx_buffer(void);

#ifdef GPIO_CAN0_TX
extern twai_node_handle_t stCANBus0;
#endif
#ifdef GPIO_CAN1_TX
extern twai_node_handle_t stCANBus1;
#endif

#define SFRCAN
#endif