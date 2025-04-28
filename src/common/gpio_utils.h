#ifndef GPIO_UTILS_H
#define GPIO_UTILS_H

#include "pico/stdlib.h"
#include <stdint.h>

#define DB_MIN 0  // GPIO first data bus pin
#define DB_MAX 15 // GPIO last data bus pin : must be contiguous

#define CS_PIN 26  // Chip select
#define RD_PIN 27  // Read enable
#define BUSY_PIN 31  // Busy (ADC is converting)
#define CONVST_PIN 30  // Start conversion

#define READ_DELAY_US 1  // Set 1MHz reading frequency

void init_data_bus(void);
void init_adc(void);
uint16_t read_data_bus(void);
float get_voltage(void);
void init_gpio_pin(uint gpio, uint direction, bool initial_value);

#endif
