add_library(common STATIC gpio_utils.c)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(common pico_stdlib hardware_dma hardware_spi)