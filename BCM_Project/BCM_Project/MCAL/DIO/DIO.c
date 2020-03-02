/*
 * DIO.c
 *
 * Created: 12/22/2019 2:11:53 PM
 *  Author: Johnny
 */ 


/********************************************************/
/*					Files Includes						*/
/********************************************************/

#include "DIO.h"
#include "registers.h"


ERROR_STATUS DIO_init (DIO_Cfg_s *DIO_info)
{
	if (DIO_info==NULL)
	{
		return E_NOK;
	}
//------------------------------------------------------------------------------------------------------//	
	else if ((DIO_info->pins)>FULL_PORT) // invalid pins input (more than the full port value)
	{
		return E_NOK;
	}
//------------------------------------------------------------------------------------------------------//	
	else
	{
		switch (DIO_info->GPIO) // to determine the port
		{
			case GPIOA:
			switch (DIO_info->dir) // to determine the direction of the port
			{
				case OUTPUT:
				PORTA_DIR|=DIO_info->pins; // setting the pins in the DDR
				break;
				
				case INPUT:
				PORTA_DIR&=~(DIO_info->pins);//clearing the pins in the DDR
				break;
				
				default: // invalid direction
				return E_NOK;
				break;
			}
			break;
//------------------------------------------------------------------------------------------------------//
			case GPIOB:
			switch (DIO_info->dir)
			{
				case OUTPUT:
				PORTB_DIR|=DIO_info->pins; // setting the pins in the DDR
				break;
				
				case INPUT:
				PORTB_DIR&=~(DIO_info->pins);//clearing the pins in the DDR
				break;
				
				default:
				return E_NOK;
				break;
			}
			break;
//------------------------------------------------------------------------------------------------------//
			case GPIOC:
			switch (DIO_info->dir)
			{
				case OUTPUT:
				PORTC_DIR|=DIO_info->pins;// setting the pins in the DDR
				break;
				
				case INPUT:
				PORTC_DIR&=~(DIO_info->pins);//clearing the pins in the DDR
				break;
				
				default:
				return E_NOK;
				break;
			}
			break;
//------------------------------------------------------------------------------------------------------//
			case GPIOD:
			switch (DIO_info->dir)
			{
				case OUTPUT:
				PORTA_DIR|=DIO_info->pins; // setting the pins in the DDR
				break;
				
				case INPUT:
				PORTC_DIR&=~(DIO_info->pins);//clearing the pins in the DDR
				break;
				
				default:
				return E_NOK;
				break;
			}
			break;
//------------------------------------------------------------------------------------------------------//
			default:
			return E_NOK;
			break;
		}
	}
	return E_OK;
}

ERROR_STATUS DIO_Write (uint8 GPIO, uint8 pins, uint8 value)
{
	switch(GPIO)
	{
		case GPIOA:
			if (value==HIGH)
			{
				PORTA_DATA|=pins;
			}
			else if (value==LOW)
			{
				PORTA_DATA&=~(pins);
			}
		break;
//------------------------------------------------------------------------------------------------------//
		case GPIOB:
			if (value==HIGH)
			{
				PORTB_DATA|=pins;
			}
			else if (value==LOW)
			{
				PORTB_DATA&=~(pins);
			}
		break;
//------------------------------------------------------------------------------------------------------//
		case GPIOC:
			if (value==HIGH)
			{
				PORTC_DATA|=pins;
			}
			else if (value==LOW)
			{
				PORTC_DATA&=~(pins);
			}
		break;
//------------------------------------------------------------------------------------------------------//
		case GPIOD:
			if (value==HIGH)
			{
				PORTD_DATA|=pins;
			}
			else if (value==LOW)
			{
				PORTD_DATA&=~(pins);
			}
		break;
//------------------------------------------------------------------------------------------------------//
		default:
		return E_NOK;
		break;
	}	
	return E_OK;
}

ERROR_STATUS DIO_Read (uint8 GPIO,uint8 pins, uint8 *data)
{
	if (data==NULL)
	{
		return E_NOK;
	}
//------------------------------------------------------------------------------------------------------//
	else 
	{
		switch(GPIO)
		{
			case GPIOA:
			*data=(PORTA_PIN&pins)/pins;
			break;
//------------------------------------------------------------------------------------------------------//
			case GPIOB:
			*data=(PORTB_PIN&pins)/pins;
			break;
//------------------------------------------------------------------------------------------------------//
			case GPIOC:
			*data=(PORTC_PIN&pins)/pins;
			break;
//------------------------------------------------------------------------------------------------------//
			case GPIOD:
			*data=(PORTD_PIN&pins)/pins;
			break;
//------------------------------------------------------------------------------------------------------//			
			default:
			return E_NOK;
			break;
		}	
	}
	return E_OK;
}

ERROR_STATUS DIO_Toggle (uint8 GPIO, uint8 pins)
{
	switch (GPIO)
	{
		case GPIOA:
		PORTA_DATA^=pins;
		break;
//------------------------------------------------------------------------------------------------------//
		case GPIOB:
		PORTB_DATA^=pins;
		break;
//------------------------------------------------------------------------------------------------------//
		case GPIOC:
		PORTC_DATA^=pins;
		break;
//------------------------------------------------------------------------------------------------------//
		case GPIOD:
		PORTD_DATA^=pins;
		break;
//------------------------------------------------------------------------------------------------------//
		default:
		return E_NOK;
		break;
	}
	return E_OK;
}