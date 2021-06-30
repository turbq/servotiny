/*
 * servotiny.c
 *
 * Created: 09.12.2019 15:35:33
 * Author : Legkiy
 */ 

/* Release v1.0.0 */

#include "common.h"

FUSES =
{
	.low = LFUSE_DEFAULT,
	.high = HFUSE_DEFAULT,	//SPIEn
};

LOCKBITS = LOCKBITS_DEFAULT;

int main(void)
{
	hw_init();
    /* Replace with your application code */
    while (1) 
    {
		wdt_reset();
		sleep_mode();
    }
}

