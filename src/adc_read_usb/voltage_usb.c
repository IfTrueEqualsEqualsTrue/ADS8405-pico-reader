#include "pico/stdlib.h"
#include <stdio.h>
#include "gpio_utils.h"

#define SIMULATOR_PIN 26

int main() {
    stdio_usb_init(); // Use the usb output as stdout 

    gpio_init(SIMULATOR_PIN);
    gpio_set_dir(SIMULATOR_PIN, GPIO_OUT);
    gpio_put(SIMULATOR_PIN, 1);  // Always HIGH

    sleep_ms(2000); // 2 sec delay
    printf("Pico is ready !\n");

    init_data_bus();

    while (true) {

        float voltage = get_voltage();
        
        printf("Voltage: %.4f V\n", voltage);

        sleep_ms(500);
    }
}
