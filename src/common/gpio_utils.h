#ifndef GPIO_UTILS_H
#define GPIO_UTILS_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include <stdint.h>

// Data bus
#define DB_MIN 0   // First GPIO for ADC data bus (GPIO0)
#define DB_MAX 15  // Last GPIO for ADC data bus (GPIO15)  TODO : investigate SIO

//ADC
#define CONVST_PIN 19  // GPIO19 - Start ADC conversion (CONVST)
#define CS_PIN 20  // GPIO20 - ADC Chip Select (CS)
#define RD_PIN 21      // GPIO21 - ADC Read Enable (RD)
#define BUSY_PIN 26    // GPIO26 - ADC Busy Status Input (BUSY)

#define READ_DELAY_US 1

//SPI
#define SPI_PORT spi0

#define PIN_MISO 16    // GPIO16 - SPI MISO (Master In Slave Out)
#define PIN_SPI_CS 17  // GPIO17 - SPI CS (Chip Select for Pi)
#define PIN_SCK 18     // GPIO18 - SPI Clock (SCK)
#define PIN_MOSI 22    // GPIO22 - SPI MOSI (Master Out Slave In)

// DMA
extern int dma_chan;
extern dma_channel_config dma_cfg;

// ADC control and data
void init_data_bus(void);
void init_adc(void);
uint16_t read_data_bus(void);
float get_voltage(void);

// SPI and DMA
void spi_init_master(void);
void dma_init_for_spi(void);

#endif
