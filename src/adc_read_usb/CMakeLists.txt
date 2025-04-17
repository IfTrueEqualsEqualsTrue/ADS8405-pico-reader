add_executable(pico-firmware pico-firmware.c )

pico_set_program_name(pico-firmware "pico-firmware")
pico_set_program_version(pico-firmware "0.1")

pico_enable_stdio_uart(pico-firmware 0)
pico_enable_stdio_usb(pico-firmware 1)

target_link_libraries(pico-firmware
        pico_stdlib)

target_include_directories(pico-firmware PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(pico-firmware)

