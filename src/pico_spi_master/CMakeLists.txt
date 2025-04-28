add_executable(pico-spi-master pico_spi_master.c)

pico_enable_stdio_usb(pico-spi-master 1)
pico_enable_stdio_uart(pico-spi-master 0)

target_link_libraries(pico-spi-master pico_stdlib hardware_spi common)

pico_add_extra_outputs(pico-spi-master)
