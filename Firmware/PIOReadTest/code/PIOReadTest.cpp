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

#define STPin   8
#define ClkPin  9
#define DataPins 2

char buffer[100];
uint32_t dataIn[2000];

int readercounter = 2600;

 void  core1_main()
 	{
		int i,j,k;
		uint32_t tmp;
		char bits;
		char tmpstring[9] = "00000000";

	printf("hello from core1\n");
		while(true)
		{
			//printf("entered loop \n");
			while(!pio_interrupt_get(pio0,1))  //wait till we started cycle
				printf(".");
			printf("\r");
			for(i = 0;i<500;i++)
			{
				//printf("entered loop 2: %d \n", i);
				dataIn[i] = pio_sm_get_blocking(pio0, 1);
				//printf("data = %d \n",dataIn[i]);
				if (dataIn[i] == readercounter) //look for the known end pattern
				{
					printf(" :%d: ",i);
					break;
				}	
			}
			//printf("filled datain \n");
			for(k = 0;k<i;k=k+1)
			{
				tmp = dataIn[k];
				/*if (tmp == readercounter) //look for the known pattern
				{
					printf("end\n");
					continue;
				}*/
				for(j = 0;j<8; j++ )
				{
					bits = tmp & 0xf;
					if (bits == 0b0000)
						tmpstring[j] = '0';
					else if (bits == 0b1000)
						tmpstring[j] = '1';
					else if (bits == 0b1100)
						tmpstring[j] = '2';
					else if (bits == 0b1110)
						tmpstring[j] = '3';
					else if (bits == 0b1111)
						tmpstring[j] = '4';
					else 
						tmpstring[j] = 'x'; //something strange came in
					tmp = tmp >> 4;
				}

				//printf("%8s ",tmpstring);
			}
		};
 	};


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

int main() {
	char dummy[100];
    set_sys_clock_khz(150000,true); 
    stdio_init_all();

	printf("started, press enter\n");
	scanf("%100s",dummy);
    // todo get free sm
    PIO pio = pio0;
    int sm0 = 0;
	int sm1 = 1;
	
    uint offset = pio_add_program(pio, &pwm_program);
	uint offsetReader = pio_add_program(pio, &reader_program);

    printf("Loaded pwm program at %d\n", offset);
    printf("Loaded reader program at %d\n", offsetReader);

	reader_program_init(pio, sm1, offsetReader, ClkPin, DataPins);
    pwm_program_init(pio, sm0, offset, STPin);

    int level = 1000;
    int period = 2600;
//	int readercounter = 2600;
    pio_pwm_set_period(pio, sm0, period);
    pio_pwm_set_level(pio, sm0, level);
	pio_pwm_set_level(pio, sm1, readercounter); 

	printf("pio initialized starting core1\n");
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


//        level = (level + 1) % 2550;
//        sleep_ms(10);
    }
}
