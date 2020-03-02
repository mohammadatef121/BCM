/*
 * Timer.c
 *
 * Created: 12/22/2019 3:34:15 PM
 *  Author: Johnny
 */ 


/********************************************************/
/*					File Includes						*/
/********************************************************/

#include "Timer.h"
#include "DIO.h"
#include "registers.h"
#include "interrupts.h"



/********************************************************/
/*				 Global variables		        		*/
/********************************************************/

static uint8  gsu8_TCCRT0_Prescaler;
static uint16 gsu16_TCCRT1_Prescaler;
static uint8  gsu8_TCCRT2_Prescaler;


/********************************************************/
/*			CallBack Functions prototypes	        	*/
/********************************************************/

static void (*sga_Timer0CBF)(void)={NULL};
static void (*sga_Timer1CBF)(void)={NULL};
static void (*sga_Timer2CBF)(void)={NULL};
	

/********************************************************/
/*						Macros			        		*/
/********************************************************/

#define TOV0  0x01 
#define OCF0  0x02
#define TOV1  0x04
#define OCF1A 0x10
#define TOV2  0x40
#define OCF2  0x80

/**
 * Input: Pointer to a structure contains the information needed to initialize the timer. 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the module.
 * 							
 */

ERROR_STATUS Timer_Init(Timer_cfg_s* Timer_cfg)
{
	switch (Timer_cfg->Timer_CH_NO)
	{
		case TIMER_CH0:
		sga_Timer0CBF=Timer_cfg->Timer_Cbk_ptr;
			switch(Timer_cfg->Timer_Mode)
			{
				case TIMER_MODE:
					switch (Timer_cfg->Timer_Prescaler)
					{
						case TIMER_PRESCALER_NO:
						gsu8_TCCRT0_Prescaler=0x01;
						break;
//------------------------------------------------------------------------------------------------------//
						case TIMER_PRESCALER_8:
						gsu8_TCCRT0_Prescaler=0x02;
						break;
//------------------------------------------------------------------------------------------------------//
						case TIMER_PRESCALER_64:
						gsu8_TCCRT0_Prescaler=0x03;
						break;
//------------------------------------------------------------------------------------------------------//
						case TIMER_PRESCALER_256:
						gsu8_TCCRT0_Prescaler=0x04;
						break;
//------------------------------------------------------------------------------------------------------//
						case TIMER_PRESCALER_1024:
						gsu8_TCCRT0_Prescaler=0x05;
						break;
//------------------------------------------------------------------------------------------------------//						
						default:
						return E_NOK;
						break;
					}
				break;
//------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------//
				case COUNTER_UP_MODE:
				gsu8_TCCRT0_Prescaler=0x07;
				break;
				case COUNTER_DOWN_MODE:
				gsu8_TCCRT0_Prescaler=0x06;
				break;
//------------------------------------------------------------------------------------------------------//						
				default:
				return E_NOK;
				break;			
			}
//------------------------------------------------------------------------------------------------------//			
		switch (Timer_cfg->Timer_Polling_Or_Interrupt)
		{
			case TIMER_POLLING_MODE:
			TIMSK&=~(1);
			break;
			case TIMER_INTERRUPT_MODE:
			TIMSK|=(1);
			break;
//------------------------------------------------------------------------------------------------------//			
			default:
			return E_NOK;
			break;
		}					
		break;

//------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------//
		case TIMER_CH1:
		sga_Timer1CBF=Timer_cfg->Timer_Cbk_ptr;
		switch(Timer_cfg->Timer_Mode)
		{
			case TIMER_MODE:
			switch (Timer_cfg->Timer_Prescaler)
			{
				case TIMER_PRESCALER_NO:
				gsu16_TCCRT1_Prescaler=0x01;
				break;
//------------------------------------------------------------------------------------------------------//
				case TIMER_PRESCALER_8:
				gsu16_TCCRT1_Prescaler=0x02;
				break;
//------------------------------------------------------------------------------------------------------//
				case TIMER_PRESCALER_64:
				gsu16_TCCRT1_Prescaler=0x03;
				break;
//------------------------------------------------------------------------------------------------------//
				case TIMER_PRESCALER_256:
				gsu16_TCCRT1_Prescaler=0x04;
				break;
//------------------------------------------------------------------------------------------------------//
				case TIMER_PRESCALER_1024:
				gsu16_TCCRT1_Prescaler=0x05;
				break;
//------------------------------------------------------------------------------------------------------//				
				default:
				return E_NOK;
				break;
			}
			break;
//------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------//
			case COUNTER_UP_MODE:
			gsu16_TCCRT1_Prescaler=0x0007;
			break;
			case COUNTER_DOWN_MODE:
			gsu16_TCCRT1_Prescaler=0x0006;
			break;
//------------------------------------------------------------------------------------------------------//			
			default:
			return E_NOK;
			break;
		}
//------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------//
		switch (Timer_cfg->Timer_Polling_Or_Interrupt)
		{
			case TIMER_POLLING_MODE:
			TIMSK=0x00;
			break;
//------------------------------------------------------------------------------------------------------//			
			case TIMER_INTERRUPT_MODE:
			TIMSK|=(1<<2);
			break;
//------------------------------------------------------------------------------------------------------//			
			default:
			return E_NOK;
			break;
		}
	break;
//------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------//
		case TIMER_CH2:
		sga_Timer2CBF=Timer_cfg->Timer_Cbk_ptr;
			switch(Timer_cfg->Timer_Mode)
			{
				case TIMER_MODE:
					switch (Timer_cfg->Timer_Prescaler)
					{
						case TIMER_PRESCALER_NO:
						gsu8_TCCRT2_Prescaler=0x01;
						break;
//------------------------------------------------------------------------------------------------------//
						case TIMER_PRESCALER_8:
						gsu8_TCCRT2_Prescaler=0x02;
						break;
//------------------------------------------------------------------------------------------------------//				
						case TIMER_PRESCALER_32:
						gsu8_TCCRT2_Prescaler=0x03;
//------------------------------------------------------------------------------------------------------//
						case TIMER_PRESCALER_64:
						gsu8_TCCRT2_Prescaler=0x04;
						break;
//------------------------------------------------------------------------------------------------------//				
						case TIMER_PRESCALER_128:
						gsu8_TCCRT2_Prescaler=0x05;
//------------------------------------------------------------------------------------------------------//
						case TIMER_PRESCALER_256:
						gsu8_TCCRT2_Prescaler=0x06;
						break;
//------------------------------------------------------------------------------------------------------//
						case TIMER_PRESCALER_1024:
						gsu8_TCCRT2_Prescaler=0x07;
						break;
//------------------------------------------------------------------------------------------------------//						
						default:
						return E_NOK;
						break;
					}
					break;
				}
//------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------//
			switch (Timer_cfg->Timer_Polling_Or_Interrupt)
			{
				case TIMER_POLLING_MODE:
				TIMSK=0x00;
				break;
//------------------------------------------------------------------------------------------------------//				
				case TIMER_INTERRUPT_MODE:
				TIMSK=0x40;
				break;
//------------------------------------------------------------------------------------------------------//				
				default:
				return E_NOK;
				break;
			}
		break;
	}
	return E_OK;
}

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to be started.
 *	Timer_Count: The start value of the timer.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function strats the needed timer.
 * 							
 */

ERROR_STATUS Timer_Start(uint8 Timer_CH_NO, uint16 Timer_Count)
{
	switch (Timer_CH_NO)
	{
		case TIMER_CH0:
		TCNT0=256 - Timer_Count;
		TCCR0=gsu8_TCCRT0_Prescaler;
		break;
//------------------------------------------------------------------------------------------------------//		
		case TIMER_CH1:
		TCNT1=65536 -Timer_Count;
		TCCR1=gsu16_TCCRT1_Prescaler;
		break;
//------------------------------------------------------------------------------------------------------//		
		case TIMER_CH2:
		TCNT2=256 - Timer_Count;
		TCCR2=gsu8_TCCRT2_Prescaler;
		break;
//------------------------------------------------------------------------------------------------------//		
		default:
		return E_NOK;
		break;
	}
	return E_OK;
}

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to be stopped.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function stops the needed timer.
 * 							
 */

ERROR_STATUS Timer_Stop(uint8 Timer_CH_NO)
{
	switch (Timer_CH_NO)
	{
		case TIMER_CH0:
		TCCR0&=0b11111000;
		break;
//------------------------------------------------------------------------------------------------------//
		case TIMER_CH1:
		TCCR1&=0b11111000;
		break;
//------------------------------------------------------------------------------------------------------//
		case TIMER_CH2:
		TCCR2&=0b11111000;
		break;
//------------------------------------------------------------------------------------------------------//
		default:
		return E_NOK;
		break;	
	}
		return E_OK;
}


/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to get its status.
 * Output:
 * 	Data: A variable of type bool returns if the flag of the timer is raised or not.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return if the flag of the timer is raised or not.
 * 							
 */

ERROR_STATUS Timer_GetStatus(uint8 Timer_CH_NO, bool* Data)
{
	switch (Timer_CH_NO)
	{
		case TIMER_CH0:
		*Data=(TIFR&TOV0)/TOV0;
		break;
//------------------------------------------------------------------------------------------------------//
		case TIMER_CH1:
		*Data=(TIFR&TOV1)/TOV1;
		break;
//------------------------------------------------------------------------------------------------------//
		case TIMER_CH2:
		*Data=(TIFR&TOV2)/TOV2;
		break;
//------------------------------------------------------------------------------------------------------//
		default:
		return E_NOK;
		break;
	}
	return E_OK;
}

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to get its value.
 * Output:
 * 	Data: This is the output variable of the function which holds the value of the timer.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return the value of the timer.
 * 							
 */

ERROR_STATUS Timer_GetValue(uint8 Timer_CH_NO, uint16* Data)
{
	switch (Timer_CH_NO)
	{
		case TIMER_CH0:
		*Data=TCNT0;
		break;
//------------------------------------------------------------------------------------------------------//
		case TIMER_CH1:
		*Data=TCNT1;
		break;
//------------------------------------------------------------------------------------------------------//
		case TIMER_CH2:
		*Data=TCNT2;
		break;
//------------------------------------------------------------------------------------------------------//
		default:
		return E_NOK;
		break;
	}
	return E_OK;
}

/********************************************************/
/*			CallBack Functions definition	        	*/
/********************************************************/

ISR (TIMER0_OVF_Vect)
{
	if (sga_Timer0CBF!=NULL)
	{
		(*sga_Timer0CBF)();
	}
}
//------------------------------------------------------------------------------------------------------//
ISR (TIMER1_OVF_Vect)
{
	if (sga_Timer1CBF!=NULL)
	{
		(*sga_Timer1CBF)();
	}
}
//------------------------------------------------------------------------------------------------------//
ISR (TIMER2_OVF_Vect)
{
	if (sga_Timer2CBF!=NULL)
	{
		(*sga_Timer2CBF)();
	}
}
//------------------------------------------------------------------------------------------------------//
