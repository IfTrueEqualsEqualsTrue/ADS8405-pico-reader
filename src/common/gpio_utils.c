#include "gpio_utils.h"
#include "pico/stdlib.h"

#define DB_MIN 0
#define DB_MAX 15

#define READ_DELAY_US 1

#define CS_PIN 26
#define RD_PIN 27
#define BUSY_PIN 31
#define CONVST_PIN 30

void init_data_bus(void) {
    for (int i = DB_MIN; i <= DB_MAX; i++) {
        gpio_init(i);
        gpio_set_dir(i, GPIO_IN);
    }
}

void init_adc() {
    gpio_init(CS_PIN); gpio_set_dir(CS_PIN, GPIO_OUT);
    gpio_init(RD_PIN); gpio_set_dir(RD_PIN, GPIO_OUT);
    gpio_init(CONVST_PIN); gpio_set_dir(CONVST_PIN, GPIO_OUT);
    gpio_init(BUSY_PIN); gpio_set_dir(BUSY_PIN, GPIO_IN);
}

uint16_t read_data_bus(void) {
    uint16_t value = 0;
    for (int i = DB_MIN; i <= DB_MAX; i++) {
        value |= (gpio_get(i) << i);
    }
    return value;
}

float get_voltage(void) {

    gpio_put(CS_PIN, 0);
    gpio_put(CONVST_PIN, 0);
    sleep_us(READ_DELAY_US);
    gpio_put(CONVST_PIN, 1);

    while (gpio_get(BUSY_PIN));

    gpio_put(RD_PIN, 0);
    uint16_t value = read_data_bus();
    gpio_put(RD_PIN, 1);
    gpio_put(CS_PIN, 1);

    float voltage = (value / 65535.0f) * 4.096f;

    return voltage;
}


