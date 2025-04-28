add_executable(voltage_usb voltage_usb.c )

pico_set_program_name(voltage_usb "voltage_usb")
pico_set_program_version(voltage_usb "0.1")

pico_enable_stdio_uart(voltage_usb 0)
pico_enable_stdio_usb(voltage_usb 1)

target_link_libraries(voltage_usb
        pico_stdlib     common)

target_include_directories(voltage_usb PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(voltage_usb)

