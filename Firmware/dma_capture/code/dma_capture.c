/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
// For ADC input:
#include "hardware/adc.h"
#include "hardware/dma.h"
// For resistor DAC output:
#include "pico/multicore.h"
#include "hardware/pio.h"
#include "resistor_dac.pio.h"

// This example uses the DMA to capture many samples from the ADC.
//
// - We are putting the ADC in free-running capture mode at 0.5 Msps
//
// - A DMA channel will be attached to the ADC sample FIFO
//
// - Configure the ADC to right-shift samples to 8 bits of significance, so we
//   can DMA into a byte buffer
//
// This could be extended to use the ADC's round robin feature to sample two
// channels concurrently at 0.25 Msps each.
//
// It would be nice to have some analog samples to measure! This example also
// drives waves out through a 5-bit resistor DAC, as found on the reference
// VGA board. If you have that board, you can take an M-F jumper wire from
// GPIO 26 to the Green pin on the VGA connector (top row, next-but-rightmost
// hole). Or you can ignore that part of the code and connect your own signal
// to the ADC input.

// Channel 0 is GPIO26
#define CAPTURE_CHANNEL 2
#define CAPTURE_DEPTH 5200

uint8_t capture_buf[CAPTURE_DEPTH];

bool Capturing = false;

void core1_main();

int main() {
    char tmp;
    stdio_init_all();

    // Send core 1 off to start driving the "DAC" whilst we configure the ADC.
    multicore_launch_core1(core1_main);

    // Init GPIO for analogue use: hi-Z, no pulls, disable digital input buffer.
    adc_gpio_init(26 + CAPTURE_CHANNEL);

    adc_init();
    adc_select_input(CAPTURE_CHANNEL);
    adc_fifo_setup(
        true,    // Write each completed conversion to the sample FIFO
        true,    // Enable DMA data request (DREQ)
        1,       // DREQ (and IRQ) asserted when at least 1 sample present
        false,   // We won't see the ERR bit because of 8 bit reads; disable.
        true     // Shift each sample to 8 bits when pushing to FIFO
    );

    // Divisor of 0 -> full speed. Free-running capture with the divider is
    // equivalent to pressing the ADC_CS_START_ONCE button once per `div + 1`
    // cycles (div not necessarily an integer). Each conversion takes 96
    // cycles, so in general you want a divider of 0 (hold down the button
    // continuously) or > 95 (take samples less frequently than 96 cycle
    // intervals). This is all timed by the 48 MHz ADC clock.
    adc_set_clkdiv(0);

    printf("Arming DMA\n");
    sleep_ms(1000);
    // Set up the DMA to start transferring data as soon as it appears in FIFO
    uint dma_chan = dma_claim_unused_channel(true);
    dma_channel_config cfg = dma_channel_get_default_config(dma_chan);

    // Reading from constant address, writing to incrementing byte addresses
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);

    // Pace transfers based on availability of ADC samples
    channel_config_set_dreq(&cfg, DREQ_ADC);
while(true)
{
    dma_channel_configure(dma_chan, &cfg,
        capture_buf,    // dst
        &adc_hw->fifo,  // src
        CAPTURE_DEPTH,  // transfer count
        true            // start immediately
    );

    printf("Starting capture\n");
    adc_run(true);

    // Once DMA finishes, stop any new conversions from starting, and clean up
    // the FIFO in case the ADC was still mid-conversion.
    dma_channel_wait_for_finish_blocking(dma_chan);
    printf("Capture finished\n");
    adc_run(false);
    adc_fifo_drain();

    bool once = true;
    int startpos = 0;

    // Print samples to stdout so you can display them in pyplot, excel, matlab
    for (int i = 0; i < CAPTURE_DEPTH; ++i) {
        tmp = capture_buf[i];
        if ((tmp >= 55)&& once) //synchronisation not perfect, need to find first sample
        {  
            printf("\n Startpos: %d \n DataStart:", i);
            once = false;
            startpos = i;
        }

        if (!once ) //&& i< (startpos + 2497))
        {    //printf("%02X;", tmp);
            fwrite(&capture_buf[startpos],1,2497, stdout); //debug to usb
            break;
        }

        
        // if (i % 60 == 59)
            // printf("\n");
    }
    printf(":DataEnd\n");

char dummy[12];
scanf("%s",dummy);
Capturing = true;
}
}

// ----------------------------------------------------------------------------
// Code for driving the "DAC" output for us to measure

// Core 1 is just going to sit and drive samples out continously. PIO provides
// consistent sample frequency.

#define OUTPUT_FREQ_HZ 1000000
#define OUT_SAMPLES 325
// This is the green channel on the VGA board
#define OUTPUT_PIN_BASE 8

#define ONPHASE 6

uint32_t OutSamples[OUT_SAMPLES];

void core1_main() {
    PIO pio = pio0;
    uint sm = pio_claim_unused_sm(pio0, true);
    uint offset = pio_add_program(pio0, &resistor_dac_2bit_program);
    resistor_dac_2bit_program_init(pio0, sm, offset, OUTPUT_FREQ_HZ, OUTPUT_PIN_BASE);
    // int counter = 0;
    int i;
    for(i=0; i<ONPHASE; i++)
        OutSamples[i] = 0xDDDDDDDD;
    for(i=ONPHASE;i<OUT_SAMPLES;i++)
        OutSamples[i] = 0x88888888;


    while (true) {
        if (Capturing)
        {
            // outputting signal
            for (i = 0; i < OUT_SAMPLES; ++i)
                pio_sm_put_blocking(pio, sm, OutSamples[i]);
            Capturing = false; // only capture once
        }
    }
}


