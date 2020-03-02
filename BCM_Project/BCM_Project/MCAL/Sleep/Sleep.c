/*
 * Sleep.c
 *
 * Created: 1/12/2020 1:09:01 PM
 *  Author: Johnny
 */ 

#include "Sleep.h"
#include "registers.h"

ERROR_STATUS Sleep_Init()
{
	MCUCR|=(1<<7);
	return E_OK;
}

ERROR_STATUS Sleep_Start(uint8 Mode)
{
	switch(Mode)
	{
		case Idle:
		MCUCR|=Idle;
		break;
		
		case ADC_Noise_Reduction:
		MCUCR|=ADC_Noise_Reduction;
		break;
		
		case Power_Down:
		MCUCR|=Power_Down;
		break;
		
		case Power_Save:
		MCUCR|=Power_Save;
		break;
		
		case Standby:
		MCUCR|=Standby;
		break;

		case Extended_Standby:
		MCUCR|=Extended_Standby;
		break;	
		
		default:
		return E_NOK;
		break;	
	}
	
	asm ("SLEEP");
	return E_OK;
}

