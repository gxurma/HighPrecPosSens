/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "PIOReadTest.pio.h"
#include "pico/bootrom.h"
#include <string.h>
#include <stdlib.h>
#include "pico/multicore.h"
#include "hardware/dma.h"
#include "hardware/structs/bus_ctrl.h"


#define STPin   8
#define ClkPin  9
#define DataPins 2

char buffer[100];
uint32_t dataIn1[650]; //2 volle cyklen passen da rein
uint32_t dataIn2[650]; //2 volle cyklen passen da rein

int dma_chan;
// todo get free sm
PIO pio = pio0;
int sm0 = 0;
int sm1 = 1;

int pioIrqCounter = 0;
int dmaIrqCounter = 0;


int level = 1000;
int period = 2800;
int readercounter = 2600;

int sema = 0;

int seite  = 1;


// Write `period` to the input shift register
void pio_pwm_set_period(PIO pio, uint sm, uint32_t period) {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_put_blocking(pio, sm, period);
    pio_sm_exec(pio, sm, pio_encode_pull(false, false));
    pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
    pio_sm_set_enabled(pio, sm, true);
}

// Write `level` to TX FIFO. State machine will copy this into X.
void pio_pwm_set_level(PIO pio, uint sm, uint32_t level) {
    pio_sm_put_blocking(pio, sm, level);
}


void core1_dmairq_handler()
{
    // Clear the interrupt request.
    dma_hw->ints0 = 1u << dma_chan;
    if (seite == 2)  
    {
        dma_channel_set_write_addr(dma_chan, dataIn1, false);
        dma_channel_set_trans_count(dma_chan, count_of(dataIn1), true);
        seite = 1;  //Welche Seite wird gerade bearbeitet?
    }
    else
    {
        dma_channel_set_write_addr(dma_chan, dataIn2, false);
        dma_channel_set_trans_count(dma_chan, count_of(dataIn2), true);
        seite = 2;
    }

    //printf("irq called %d\n", ++dmaIrqCounter);

}

void core1_pioirq_handler()
{
    pioIrqCounter++;
    sema = 1;
    pio_interrupt_clear(pio, 0);
    
}

void  core1_main()
{
    int i,j,k;
    uint32_t tmp;
    char bits;
    printf("hello from core1\n");

    pio_sm_set_enabled(pio, sm0, false);
    pio_sm_set_enabled(pio, sm1, false);

    pio_sm_clear_fifos(pio, sm0);
    pio_sm_restart(pio, sm0);
    pio_sm_clear_fifos(pio, sm1);
    pio_sm_restart(pio, sm1);

    dma_channel_config c = dma_channel_get_default_config(dma_chan);
    channel_config_set_read_increment(&c, false);
    channel_config_set_write_increment(&c, true);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm1, false));

    dma_channel_configure(dma_chan, &c,
        dataIn1,        // Destination pointer
        &pio->rxf[sm1],      // Source pointer
        count_of(dataIn1), // Number of transfers
        true                // Start not yet immediately
    );

    printf("Set up dma configured \n");
    //pio_sm_exec(pio, sm1, pio_encode_wait_gpio(trigger_level, trigger_pin));

    // Tell the DMA to raise IRQ line 0 when the channel finishes a block
    dma_channel_set_irq0_enabled(dma_chan, true);
    printf("Set up dma irq 0 enabled \n");

    // Configure the processor to run dma_handler() when DMA IRQ 0 is asserted
    irq_set_exclusive_handler(DMA_IRQ_0, core1_dmairq_handler);
    irq_set_enabled(DMA_IRQ_0, true);
    printf("Set up dma irq \n");

    pio_set_irq0_source_enabled(pio, pis_interrupt0, true);
    printf("Set up pio 1 \n");
    irq_set_exclusive_handler(PIO0_IRQ_0, core1_pioirq_handler);
    printf("Set up pio 2 \n");

    pio_sm_set_enabled(pio, sm0, true);
    pio_sm_set_enabled(pio, sm1, true);
    printf("Set up pio irq \n");

    irq_set_enabled(PIO0_IRQ_0, true);


    pio_pwm_set_period(pio, sm0, period);
	printf("period set, set level\n");

    pio_pwm_set_level(pio, sm0, level);

	printf("level set, set reader level\n");
	pio_pwm_set_level(pio, sm1, readercounter); 
	printf("reader level set\n");

    // Manually call the handler once, to trigger the first transfer
    // core1_dmairq_handler();

    printf("now starting looping\n");
    while(true)
    {
        // tmp = dataIn[0];
        // bits = tmp & 0b1111;
        while(sema == 0)
            {};
        sema = 0;
        if (seite == 1)  // es wird grade seite 1 bearbeitet, also arbeiten wir mit Seite 2 um nicht korrupte Daten zu sehen
        {
            for (i=0;i<650;i++)
                if (dataIn2[i] == readercounter)
                    break;
            printf("%10d %d %10d ", i, seite, pioIrqCounter);
            for(j=i;j<i+20;j++)
                if (j<650) //avoid memory overrun reading
                    printf("%08X ", dataIn2[j]);
            printf(" %d\n", seite);
        }
        else //seite == 2 wird bearbeitet vom DMA, also nehmen wir die Seite 1
        {
            for (i=0;i<650;i++)
                if (dataIn1[i] == readercounter)
                    break;
            printf("%10d %d %10d ", i, seite, pioIrqCounter);
            for(j=i;j<i+20;j++)
                if (j<650) //avoid memory overrun reading
                    printf("%08X ", dataIn1[j]);
            printf(" %d\n", seite);
        }


        // printf("%d\r",pioIrqCounter);
        // if ((pioIrqCounter > 0) && ((pioIrqCounter % 10000) == 0) )
        //     printf("\n %08X \n",tmp);
    };
};


int main() {
	char dummy[100];
    set_sys_clock_khz(150000,true); 
    stdio_init_all();

	printf("started, press enter\n");
	scanf("%100s",dummy);

    uint offset = pio_add_program(pio, &pwm_program);
	uint offsetReader = pio_add_program(pio, &reader_program);

    printf("Loaded pwm program at %d\n", offset);
    printf("Loaded reader program at %d\n", offsetReader);

    pwm_program_init(pio, sm0, offset, STPin, ClkPin);
	
    printf("pio initialized, pwmprg initialized\n");

	reader_program_init(pio, sm1, offsetReader, ClkPin, DataPins);
	printf("reader initialized\n");


	printf("pio initialized, starting core1\n");
	multicore_launch_core1(core1_main);
	printf("core1 started\n");


    while (true) {
        scanf("%100s",buffer);//blocking waiting for input fom user
        if (strcmp(buffer,"reboot") == 0)
            reset_usb_boot(25,0); //lets see on the LED pin what is happening while programming
        else if (buffer[0] == 'l')
        {
            level = atoi(&buffer[1]);
            pio_pwm_set_level(pio, sm0, level);
        }
        else if (buffer[0] == 'p')
        {
            period = atoi(&buffer[1]);
            pio_pwm_set_period(pio, sm0, period);
        }
        else if (buffer[0] == 'f')
		{
            readercounter = atoi(&buffer[1]);
            pio_pwm_set_level(pio, sm1, readercounter);  //reusing function call to set counter of reader
		}
        printf("period = %d\n", period);
        printf("Level = %d\n", level);
        printf("readercounter = %d\n", readercounter);
    }
}
