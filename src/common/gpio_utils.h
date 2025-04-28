#ifndef GPIO_UTILS_H
#define GPIO_UTILS_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include <stdint.h>


// ADC Control Pins
#define DB_MIN 0  // GPIO first data bus pin
#define DB_MAX 15 // GPIO last data bus pin (must be contiguous)

#define CS_PIN 26  // Chip Select
#define RD_PIN 27  // Read Enable
#define BUSY_PIN 31 // Busy (conversion running)
#define CONVST_PIN 30 // Start conversion

#define READ_DELAY_US 1  // Delay for read timing


// SPI Pins
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

// DMA
extern int dma_chan;
extern dma_channel_config dma_cfg;

// Functions
void init_data_bus(void);
void init_adc(void);
uint16_t read_data_bus(void);
float get_voltage(void);

void spi_init_master(void);
void dma_init_for_spi(void);

#endif
