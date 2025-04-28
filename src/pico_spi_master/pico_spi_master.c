#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "gpio_utils.h"
#include <stdbool.h>
#include <stdint.h>

#define BUFFER_SIZE 16384

volatile uint16_t buffer0[BUFFER_SIZE];
volatile uint16_t buffer1[BUFFER_SIZE];

volatile bool buffer0_ready = false;
volatile bool buffer1_ready = false;

volatile int current_buffer = 0;

void send_buffer(uint16_t* buffer) {
    spi_write_blocking(SPI_PORT, (uint8_t*)buffer, BUFFER_SIZE * sizeof(uint16_t));
}

void acquisition_loop(void) {
    int sample_index = 0;

    while (true) {
        gpio_put(ADC_CS_PIN, 0);
        gpio_put(CONVST_PIN, 0);
        sleep_us(READ_DELAY_US);
        gpio_put(CONVST_PIN, 1);

        while (gpio_get(BUSY_PIN));

        gpio_put(RD_PIN, 0);
        uint16_t value = read_data_bus();
        gpio_put(RD_PIN, 1);
        gpio_put(ADC_CS_PIN, 1);

        if (current_buffer == 0) {
            buffer0[sample_index] = value;
        } else {
            buffer1[sample_index] = value;
        }

        sample_index++;

        if (sample_index >= BUFFER_SIZE) {
            if (current_buffer == 0) {
                buffer0_ready = true;
                send_buffer((uint16_t*)buffer0);
            } else {
                buffer1_ready = true;
                send_buffer((uint16_t*)buffer1);
            }

            current_buffer = !current_buffer;
            sample_index = 0;
        }
    }
}

int main() {
    stdio_init_all();
    init_data_bus();
    init_adc();
    spi_init_master();
    
    acquisition_loop();
}
