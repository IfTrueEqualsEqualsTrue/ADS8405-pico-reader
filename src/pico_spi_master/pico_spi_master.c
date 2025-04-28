#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "gpio_utils.h"
#include <stdbool.h>
#include <stdint.h>

#define BUFFER_SIZE 1024

volatile uint16_t buffer0[BUFFER_SIZE];
volatile uint16_t buffer1[BUFFER_SIZE];

volatile bool buffer0_ready = false;
volatile bool buffer1_ready = false;

volatile int current_buffer = 0;

#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

void spi_init_master() {
    spi_init(SPI_PORT, 1000 * 1000); // 1MHz SPI
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
}

void send_buffer(uint16_t* buffer) {
    spi_write_blocking(SPI_PORT, (uint8_t*)buffer, BUFFER_SIZE * sizeof(uint16_t));
}

void acquisition_loop(void) {
    int sample_index = 0;

    while (true) {
        gpio_put(CS_PIN, 0);
        gpio_put(CONVST_PIN, 0);
        sleep_us(READ_DELAY_US);
        gpio_put(CONVST_PIN, 1);

        while (gpio_get(BUSY_PIN));

        gpio_put(RD_PIN, 0);
        uint16_t value = read_data_bus();
        gpio_put(RD_PIN, 1);
        gpio_put(CS_PIN, 1);

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

    return 0;
}
