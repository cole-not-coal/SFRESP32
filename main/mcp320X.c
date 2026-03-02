/*
mcp320X.c
File contains the MCP3204/8 interface over spi.

Written by Cole Perera for Sheffield Formula Racing 2026
*/

#include "mcp320X.h"

/* --------------------------- Local Types ---------------------------- */
spi_device_handle_t stMCP3204_1;
spi_device_handle_t stMCP3204_2;
spi_device_handle_t stMCP3208;

/* --------------------------- Local Variables ------------------------ */


/* --------------------------- Global Variables ----------------------- */


/* --------------------------- Definitions ---------------------------- */
#define MCP3208_SPI_FREQ 1000000 // 1 MHz

/* --------------------------- Function prototypes -------------------- */
esp_err_t MCP320X_init(uint8_t abyNCSPins[],spi_device_handle_t astDeviceHandles[]);
float MCP320X_read(spi_device_handle_t stDeviceHandle, uint8_t NDevADC);

/* --------------------------- Functions ------------------------------ */
esp_err_t MCP320X_init(uint8_t abyNCSPins[],spi_device_handle_t astDeviceHandles[])
{
    /*
    *===========================================================================
    *   MCP320X_init
    *   Takes:   None
    * 
    *   Returns: ESP_OK if successful, error code if not.
    * 
    *   Initializes the MCP3204/8 devices over spi.
    *===========================================================================
    *   Revision History:
    *   24/02/26 CP Initial Version
    *   02/03/26 CP Supports multiple devices
    *
    *===========================================================================
    */
    esp_err_t eStatus = ESP_OK;
    word wNCounter = 0;
    word wNDevices = sizeof(*abyNCSPins) / sizeof(abyNCSPins[0]);
    spi_device_interface_config_t stDeviceConfig = 
    {
        .clock_speed_hz = MCP3208_SPI_FREQ,
        .mode = 0,
        .command_bits = 2,
        .address_bits = 0, 
        .queue_size = 5,
    };
    for (wNCounter = 0; wNCounter < wNDevices; wNCounter++)
    {
        stDeviceConfig.spics_io_num = abyNCSPins[wNCounter];
        eStatus = spi_bus_add_device(SPI2_HOST, &stDeviceConfig, &astDeviceHandles[wNCounter]);
        if (eStatus != ESP_OK)
        {
            ESP_LOGE(SFR_TAG, "Failed to add MCP320X device %d: %s", wNCounter, esp_err_to_name(eStatus));
            return eStatus;
        }
    }
    return eStatus;
}

float MCP320X_read(spi_device_handle_t stDeviceHandle, uint8_t NDevADC)
{
    /*
    *===========================================================================
    *   MCP320X_read
    *   Takes:   stDeviceHandle: Handle to the SPI device
    *            byCommand: Command to send to the MCP3208
    *            byAddress: Address of the channel to read from
    *            pwData: Pointer to where the data will be stored
    * 
    *   Returns: ESP_OK if successful, error code if not.
    * 
    *   Sends a command and address to the MCP3208 and reads back the data.
    *===========================================================================
    *   Revision History:
    *   02/03/26 CP Initial Version
    *
    *===========================================================================
    */
    esp_err_t eStatus = ESP_OK;
    spi_transaction_t stSPICall;
    word wNData;
    float fVADCRaw;
    uint8_t NCmd = 0xF & (8 + (NDevADC & 0x7));
    if (NDevADC > 7)
    {
        return 999.9;
    }
    memset(&stSPICall, 0, sizeof(stSPICall));
    stSPICall.length = 4 + 13;
    stSPICall.tx_buffer = &NCmd;
    stSPICall.rx_buffer = &wNData;
    eStatus = spi_device_transmit(stDeviceHandle, &stSPICall);
    if (eStatus != ESP_OK)    {
        return 999.9;
    }
    wNData = 0xFFF & wNData;
    fVADCRaw = (float)wNData / 4096.0 * 5.0;
    ESP_LOGI(SFR_TAG, "ADC Reading: %f   | %d", fVADCRaw, wNData);
    return fVADCRaw;

}