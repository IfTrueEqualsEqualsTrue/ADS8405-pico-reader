add_executable(pico-spi-master-dma pico_spi_master_dma.c)

pico_enable_stdio_usb(pico-spi-master-dma 1)
pico_enable_stdio_uart(pico-spi-master-dma 0)

target_link_libraries(pico-spi-master-dma pico_stdlib hardware_spi hardware_dma common)

pico_add_extra_outputs(pico-spi-master-dma)
