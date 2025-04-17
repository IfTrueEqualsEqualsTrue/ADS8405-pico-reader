add_executable(pico-spi-slave pico_spi_slave.c)

pico_enable_stdio_usb(pico-spi-slave 1)
pico_enable_stdio_uart(pico-spi-slave 0)

target_link_libraries(pico-spi-slave pico_stdlib hardware_spi)

pico_add_extra_outputs(pico-spi-slave)
