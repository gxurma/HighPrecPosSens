add_executable(code 
        PIOReadTest.cpp)

pico_generate_pio_header(code ${CMAKE_CURRENT_LIST_DIR}/PIOReadTest.pio)


target_link_libraries(code PRIVATE pico_stdlib hardware_pio pico_multicore)

pico_enable_stdio_uart(code 0)
pico_enable_stdio_usb(code 1)

pico_add_extra_outputs(code)
