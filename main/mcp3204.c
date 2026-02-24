/*
mcp3204.c
File contains the MCP3204 interface over spi.

Written by Cole Perera for Sheffield Formula Racing 2026
*/

#include "mcp3204.h"

/* --------------------------- Local Types ---------------------------- */


/* --------------------------- Local Variables ------------------------ */


/* --------------------------- Global Variables ----------------------- */


/* --------------------------- Definitions ---------------------------- */
#define MCP3204_SPI_FREQ 1000000 // 1 MHz

/* --------------------------- Function prototypes -------------------- */
esp_err_t MCP3204_init(void);

/* --------------------------- Functions ------------------------------ */
esp_err_t MCP3204_init(void)
{
    /*
    *===========================================================================
    *   MCP3204_init
    *   Takes:   None
    * 
    *   Returns: ESP_OK if successful, error code if not.
    * 
    *   Initializes the MCP3204 interface over spi.
    *===========================================================================
    *   Revision History:
    *   24/02/26 CP Initial Version
    *
    *===========================================================================
    */
    esp_err_t eStatus = ESP_OK;
    spi_bus_config_t stBusConfig = 
    {
        .mosi_io_num = SPI_MOSI,
        .miso_io_num = SPI_MISO,
        .sclk_io_num = SPI_SCK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    spi_device_interface_config_t stDeviceConfig = 
    {
        .clock_speed_hz = MCP3204_SPI_FREQ,
        .mode = 0,
        .command_bits = 2,
        .address_bits = 0,
    };



}

