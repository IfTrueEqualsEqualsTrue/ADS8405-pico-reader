#include <stdio.h> // <-- Add this line
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "gpio_utils.h"
#include <string.h>


#define SPI_PORT spi0
#define PIN_MISO 4
#define PIN_CS   5
#define PIN_SCK  2
#define PIN_MOSI 3

uint8_t rx_buf[32];
uint8_t tx_buf[32] = "Pico ready!\n";

int main() {
    stdio_init_all();

    spi_init(SPI_PORT, 1 * 1000 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    spi_set_slave(SPI_PORT, true);

    while (true) {
        // Wait for master to send data
        size_t len = spi_read_blocking(SPI_PORT, 0x00, rx_buf, sizeof(rx_buf));

        if (len > 0) {
            printf("Received: %.*s\n", len, rx_buf);

            // Respond if needed
            for (int i = 0; i < len; i++) {
                spi_write_blocking(SPI_PORT, &tx_buf[i], 1);
            }
        }
    }
}
