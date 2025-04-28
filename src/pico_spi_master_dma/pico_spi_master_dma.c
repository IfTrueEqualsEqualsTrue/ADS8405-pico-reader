#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
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

int dma_chan;
dma_channel_config dma_cfg;

void spi_init_master() {
    spi_init(SPI_PORT, 1000 * 1000); // 1MHz SPI
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
}

void dma_init_for_spi() {
    dma_chan = dma_claim_unused_channel(true);
    dma_cfg = dma_channel_get_default_config(dma_chan);

    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_8);
    channel_config_set_read_increment(&dma_cfg, true);
    channel_config_set_write_increment(&dma_cfg, false);
    channel_config_set_dreq(&dma_cfg, spi_get_dreq(SPI_PORT, true));
}

void send_buffer_dma(uint16_t* buffer) {
    dma_channel_configure(
        dma_chan,
        &dma_cfg,
        &spi_get_hw(SPI_PORT)->dr,   // write address (SPI data register)
        (uint8_t*)buffer,            // read address (filled buffer)
        BUFFER_SIZE * sizeof(uint16_t), // number of bytes to transfer
        true                         // autostart
    );

    // Wait for DMA to finish
    dma_channel_wait_for_finish_blocking(dma_chan);
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
                send_buffer_dma((uint16_t*)buffer0);
            } else {
                buffer1_ready = true;
                send_buffer_dma((uint16_t*)buffer1);
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
    dma_init_for_spi();

    acquisition_loop();

    return 0;
}
