/*
 * SoftwareDelay.c
 *
 * Created: 12/3/2019 7:33:10 PM
 *  Author: Johnny
 */ 
#include "std_types.h"

void SwDelay_ms(uint32 n)
{
	for (uint32 i=0;i<(93*16*n);i++)
	{
		asm("nop");
	}
}

