#include "pico/stdlib.h"
#include <stdio.h>
#include "gpio_utils.h"



int main() {
    stdio_usb_init();

    sleep_ms(2000); // 2 sec delay
    printf("Hello from Pico!\n");

    init_data_bus();

    while (true) {

        float voltage = get_voltage();
        
        printf("Voltage: %.4f V\n", voltage);

        sleep_ms(500);
    }
}
