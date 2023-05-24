add_executable(code
        dma_capture.c
        )

pico_generate_pio_header(code ${CMAKE_CURRENT_LIST_DIR}/resistor_dac.pio)

target_link_libraries(code
		pico_stdlib
		hardware_adc
		hardware_dma
		# For the dummy output:
		hardware_pio
		pico_multicore
		)

pico_enable_stdio_uart(code 0)
pico_enable_stdio_usb(code 1)


# create map/bin/hex file etc.
pico_add_extra_outputs(code)

# add url via pico_set_program_url
#example_auto_set_url(code)


