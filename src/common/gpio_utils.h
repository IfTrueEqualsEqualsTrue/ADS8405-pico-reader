#ifndef GPIO_UTILS_H
#define GPIO_UTILS_H

#include <stdint.h>
#include "pico/stdlib.h"

void init_data_bus(void);
uint16_t read_data_bus(void);
void init_adc(void);
float get_voltage(void);

#endif
