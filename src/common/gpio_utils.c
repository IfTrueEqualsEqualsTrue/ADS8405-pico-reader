#include "gpio_utils.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include "hardware/structs/sio.h"

int dma_chan;
dma_channel_config dma_cfg;

void init_data_bus(void) {
    for (int i = DB_MIN; i <= DB_MAX; i++) {
        gpio_init(i);
        gpio_set_dir(i, GPIO_IN);
    }
}

void init_adc(void) {
    gpio_init(ADC_CS_PIN); gpio_set_dir(ADC_CS_PIN, GPIO_OUT);
    gpio_init(RD_PIN); gpio_set_dir(RD_PIN, GPIO_OUT);
    gpio_init(CONVST_PIN); gpio_set_dir(CONVST_PIN, GPIO_OUT);
    gpio_init(BUSY_PIN); gpio_set_dir(BUSY_PIN, GPIO_IN);
}

uint16_t read_data_bus(void) {
    // Read all GPIOs, shift to align DB0 at bit 0, mask to keep DB0..DB15 only
    return (sio_hw->gpio_in >> DB_MIN) & 0xFFFF;
}

float get_voltage(void) {
    gpio_put(ADC_CS_PIN, 0);
    gpio_put(CONVST_PIN, 0);
    sleep_us(READ_DELAY_US);
    gpio_put(CONVST_PIN, 1);

    while (gpio_get(BUSY_PIN));

    gpio_put(RD_PIN, 0);
    uint16_t value = read_data_bus();
    gpio_put(RD_PIN, 1);
    gpio_put(ADC_CS_PIN, 1);

    float voltage = (value / 65535.0f) * 4.096f;
    return voltage;
}

void spi_init_master(void) {
    spi_init(SPI_PORT, 1000 * 1000); // 1 MHz
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
}

void dma_init_for_spi(void) {
    static int dma_chan = -1;
    static dma_channel_config dma_cfg;

    dma_chan = dma_claim_unused_channel(true);
    dma_cfg = dma_channel_get_default_config(dma_chan);

    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_8);
    channel_config_set_read_increment(&dma_cfg, true);
    channel_config_set_write_increment(&dma_cfg, false);
    channel_config_set_dreq(&dma_cfg, spi_get_dreq(SPI_PORT, true));
}

