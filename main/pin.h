#ifndef SFR_PIN
#define SFR_PIN

/* All Devices */
#define GPIO_ONBOARD_LED 15

#define GPIO_CAN0_TX 22
#define GPIO_CAN0_RX 23

// #define GPIO_CAN1_TX XX
// #define GPIO_CAN1_RX XX

/* Logger only */
#define SPI_SD_CS 17
#define SPI_MOSI 18
#define SPI_SCK 19
#define SPI_MISO 20

#define I2C0_SCL 2
#define I2C0_SDA 21

/* Wheel Only */
#define EVE_PDN 16
#define EVE_CS 17
#define EVE_MOSI 18
#define EVE_SCK 19
#define EVE_MISO 20

/* IMD Monitor Only */
#define IMD_PWM_IN ADC_CHANNEL_0 // GPIO0

/* APPs Only */
#define APPS1_IN ADC_CHANNEL_0 // GPIO0
#define APPS2_IN ADC_CHANNEL_1 // GPIO1

#define SFR_TAG  "SFR_ESP32"

/* Contactor Drive Only */
#define GPIO_PRECHARGE_CONTACTOR_PWM 0 
#define GPIO_MAIN_POS_CONTACTOR_PWM 0
#define GPIO_MAIN_NEG_CONTACTOR_PWM 0
#define ADC_PRECHARGE_CAR_SIDE ADC_CHANNEL_0 // GPIO0
#define ADC_PRECHARGE_ACCU_SIDE ADC_CHANNEL_1 // GPIO1
#define ADC_MAIN_POS_CAR_SIDE ADC_CHANNEL_2 // GPIO2
#define ADC_MAIN_NEG_CAR_SIDE ADC_CHANNEL_3 // GPIO3
#define ADC_MAIN_POS_ACCU_SIDE ADC_CHANNEL_4 // GPIO4
#define ADC_MAIN_NEG_ACCU_SIDE ADC_CHANNEL_6 // GPIO6

#endif // SFR_PIN
