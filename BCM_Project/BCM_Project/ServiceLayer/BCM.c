/*
 * BCM.c
 *
 * Created: 1/13/2020 8:31:57 PM
 *  Author: Johnny
 */

/********************************************************/
/*					Files Includes						*/
/********************************************************/ 

#include "BCM.h"
#include "UART.h"
#include "registers.h"
#include "DIO.h"
#include "BCM_lcfg.h"
#include "BCM_cfg.h"


/********************************************************/
/*					Macros								*/
/********************************************************/

/*states of transmitting and receiving*/

#define IDLE			(0)
#define TX				(1)
#define TX_COMPLETE		(2)
#define RX				(3)
#define RX_COMPLETE		(4)

/*states for the ISR*/

#define LOCKED			(0)
#define UNLOCKED		(1)

/*states for the SM of the transmitting*/

#define SEND_ID					(0)
#define SEND_SIZE_HIGH			(1)
#define SEND_SIZE_LOW			(2)
#define SEND_DATA				(3)
#define SEND_CHECK_SUM			(4)

/*states for the SM of the receiving*/

#define RECEIVE_ID					(0)
#define RECEIVE_SIZE_HIGH			(1)
#define RECEIVE_SIZE_LOW			(2)
#define RECEIVE_DATA				(3)
#define RECEIVE_CHECK_SUM			(4)

#define INITIALIZED                 (0)
#define NOTINITIALIZED              (1)


/********************************************************/
/*					global variables					*/
/********************************************************/

uint8 gu8_SEND_SM=SEND_ID;
uint8 gu8_RECEIVE_SM=RECEIVE_ID;
uint16 gu16_Buffer_Receive_Size;

uint8 gu8_SM_Tx;
uint8 gu8_SM_Rx;

uint8 gu8_BCM_Init_Status=NOTINITIALIZED;

uint8 gu8_Receive_ID;
uint8 gua_Received_CheckSum=0;

UART_cfg_s gstr_UART_cfgs;

/********************************************************/
/*					struct								*/
/********************************************************/

typedef struct  {
	uint16 Buffer_Size;
	uint8* Data;
	void (*ptrfu_BCM_Cbk)(void);
}gstr_Buffer_info_t;

gstr_Buffer_info_t gstr_Buffer_Send_Info;
gstr_Buffer_info_t gstr_Buffer_Receive_Info;


typedef struct {
	uint8	Check_Sum;
	uint8   State;	
	uint8   Buffer_State;	
	uint8	ID;
	uint8   Index;
}gstr_BCM_info_t;
	
 gstr_BCM_info_t gstr_BCM_Send_info;
 gstr_BCM_info_t gstr_BCM_Receive_info;  


	

/********************************************************/
/*					Call Back Functions					*/
/********************************************************/

void BCM_TX_CBK(void)
{
	/*to make the dispatcher wait the byte to be sent*/
	if (gstr_BCM_Send_info.State==TX_COMPLETE)
	{
		gstr_BCM_Send_info.State=TX;
	}
}

void BCM_RX_CBK(void)
{
	/*to make the dispatcher wait the byte to be received*/
	if (gstr_BCM_Receive_info.State==RX_COMPLETE)
	{
		gstr_BCM_Receive_info.State=RX;
	}
}


/**************************************************************************
 * Function 	: BCM_Init                                                *
 * Input 		: Icu_cfg : void								          *
 * Return 		: value of type ERROR_STATUS							  *			  
 * Description  : initialize the BCM by initializing the UART			  *
				  and make the initiate the struct by zeros and nulls	  *
 **************************************************************************/

ERR_STATUS BCM_Init()
{
	ERR_STATUS BCM_Init_Error=ERR_SUCCESS;
	if (gu8_BCM_Init_Status==NOTINITIALIZED)
	{
		if (gstr_BCM_cfg.Peripheral==UART)
		{
			switch(gstr_BCM_cfg.MODE)
			{
				case Transmitter:
				gstr_UART_cfgs.u32_BaudRate=9600;
				gstr_UART_cfgs.u8_DataSize=UART_8_BIT;
				gstr_UART_cfgs.u8_DesiredOperation=Transmitter;
				gstr_UART_cfgs.u8_DoubleSpeed=UART_NO_DOUBLE_SPEED;
				gstr_UART_cfgs.u8_InterruptMode=UART_INTERRUPT;
				gstr_UART_cfgs.u8_ParityBit=UART_NO_PARITY;
				gstr_UART_cfgs.u8_StopBit=UART_ONE_STOP_BIT;
				gstr_UART_cfgs.ptr_Transmit_CBK=BCM_TX_CBK;
				UART_Init(&gstr_UART_cfgs);
				break;
				
				case Receiver:
				gstr_UART_cfgs.u32_BaudRate=9600;
				gstr_UART_cfgs.u8_DataSize=UART_8_BIT;
				gstr_UART_cfgs.u8_DesiredOperation=Receiver;
				gstr_UART_cfgs.u8_DoubleSpeed=UART_NO_DOUBLE_SPEED;
				gstr_UART_cfgs.u8_InterruptMode=UART_INTERRUPT;
				gstr_UART_cfgs.u8_ParityBit=UART_NO_PARITY;
				gstr_UART_cfgs.u8_StopBit=UART_ONE_STOP_BIT;
				gstr_UART_cfgs.ptr_Resceive_CBK=BCM_RX_CBK;
				UART_Init(&gstr_UART_cfgs);
				break;
				
				default:
				break;
			}
			
		}
		
		else if (gstr_BCM_cfg.Peripheral==SPI)
		{
			
		}
		
		else 
		{
			/*DO NOTHING*/
		}
		
		gstr_BCM_Send_info.State=IDLE;
		
		gstr_BCM_Send_info.Check_Sum=0;
		gstr_BCM_Receive_info.Check_Sum=0;
	
		gstr_BCM_Send_info.ID=0;
		gstr_BCM_Receive_info.ID=0;
		
		gstr_BCM_Send_info.Index=0;
		gstr_BCM_Send_info.Index=0;
		
		gstr_BCM_Send_info.Buffer_State=UNLOCKED;
		gstr_BCM_Receive_info.Buffer_State=UNLOCKED;
		
		gstr_BCM_Send_info.State=IDLE;
		gstr_BCM_Receive_info.State=IDLE;

		gstr_Buffer_Send_Info.Buffer_Size=0;
		gstr_Buffer_Receive_Info.Buffer_Size=0;
		
		gstr_Buffer_Send_Info.Data=NULL;
		gstr_Buffer_Receive_Info.Data=NULL;			
		
		gu8_BCM_Init_Status=INITIALIZED;
	}
	
	else if (gu8_BCM_Init_Status==INITIALIZED)
	{
		BCM_Init_Error=ERR_ALREADY_INITIALIZED;
	}
	
	else 
	{
		/*DO NOTHING*/
	}
	
	return BCM_Init_Error;
	
}

/**************************************************************************
 * Function 	: BCM_Send                                                *
 * Input 		: Icu_cfg : void								          *
 * Return 		: value of type ERROR_STATUS							  *			  
 * Description  : create transmitting event by declare the size			  *	
				  and array of data and assign an ID for the session	  *
 **************************************************************************/

ERR_STATUS BCM_Send(uint8* array, uint16 Size,void (*Func_Send_ptr)(void))
{
	ERR_STATUS BCM_SEND_Status=ERR_SUCCESS;
	
	if (gu8_BCM_Init_Status==INITIALIZED)
	{
		if ((array!=NULL)&&(Func_Send_ptr!=NULL))
		{
			if (Size<=MaxNumberOfTransmittedBytes)
			{
				if (gstr_BCM_Send_info.Buffer_State==UNLOCKED)
				{
					/*load the size of buffer into the struct*/
					gstr_Buffer_Send_Info.Buffer_Size=Size;
					/*load the data into the struct*/
					gstr_Buffer_Send_Info.Data=array;
					gstr_Buffer_Send_Info.ptrfu_BCM_Cbk=Func_Send_ptr;
					/*ID the data*/
					gstr_BCM_Send_info.ID=97;
				}
				/*update the state of the state machine for sending*/
				gstr_BCM_Send_info.State=TX;
				/*close the buffer till it's fully transmitted*/
				gstr_BCM_Send_info.Buffer_State=LOCKED;
				/*update the state of the sending bytes*/
				gu8_SEND_SM=SEND_ID;			
			}
			
			else if (Size<MaxNumberOfTransmittedBytes)
			{
				BCM_SEND_Status=ERR_INVALID_ARGU;
			}
			
			else 
			{
				/*DO NOTHING*/
			}
	
		}
		
		else if (array==NULL)
		{
			BCM_SEND_Status=ERR_NULL_PTR;
		}
		
		else 
		{
			/*DO NOTHING*/
		}

	}
	
	else if (gu8_BCM_Init_Status==NOTINITIALIZED)
	{
		BCM_SEND_Status=ERR_NOTINITIALIZED;
	}
	
	else 
	{
		/*DO NOTHING*/
	}
	
	return BCM_SEND_Status;
}

/**************************************************************************
 * Function 	: BCM_Setup_Receive                                       *
 * Input 		: Icu_cfg : void								          *
 * Return 		: value of type ERROR_STATUS							  *			  
 * Description  : create receiving event by declare the size			  *	
				  and array of data and assign an ID for the session	  *
 **************************************************************************/

ERR_STATUS BCM_Setup_Receive(uint8* array, uint16 Size,void (*Func_Receive_ptr)(void))
{
	ERR_STATUS BCM_Setup_Receive_Error=ERR_SUCCESS;
	
	if (gstr_BCM_Receive_info.Buffer_State==UNLOCKED)
	{
		if ((array!=NULL)&&(Func_Receive_ptr!=NULL))
		{
			/*declare the array to be saved the data into*/
			gstr_Buffer_Receive_Info.Data=array;
			/*declare the size of the to be received data*/
			gstr_Buffer_Receive_Info.Buffer_Size=Size;
			/*declare the id*/
			gu8_Receive_ID=97;
			/*start the state machine of sending*/
			gstr_BCM_Receive_info.State=RX;
			gstr_BCM_Receive_info.Buffer_State=LOCKED;
			gu8_RECEIVE_SM=RECEIVE_ID;
			gstr_Buffer_Receive_Info.ptrfu_BCM_Cbk=Func_Receive_ptr;			
		}
		
		else if ((array==NULL)&&(Func_Receive_ptr==NULL))
		{
			BCM_Setup_Receive_Error=ERR_NULL_PTR;
		}
		
		else
		{
			/*DO NOTHING*/
		}

	}
	else if (gstr_BCM_Receive_info.Buffer_State==LOCKED)
	{
		BCM_Setup_Receive_Error=ERR_ALREADY_INITIALIZED;
	}
	
	else 
	{
		/*DO NOTHING*/
	}

	return BCM_Setup_Receive_Error;
}

/**************************************************************************
 * Function 	: BCM_TxDispatcher                                        *
 * Input 		: Icu_cfg : void								          *
 * Return 		: value of type ERROR_STATUS							  *			  
 * Description  : sends a byte each ISR									  *
 **************************************************************************/


ERR_STATUS BCM_TxDispatcher()
{
	ERR_STATUS BCM_TxDispatcher_Error=ERR_SUCCESS;
	
	if (gu8_BCM_Init_Status==INITIALIZED)
	{	
		/*checks if the buffer is locked*/
		if (gstr_BCM_Send_info.Buffer_State==LOCKED)
		{
			switch (gu8_SEND_SM)
			{
				
				/*send the ID of the buffer*/
				case SEND_ID:
				if (gstr_BCM_Send_info.State==TX)
				{
					UART_SendByte( gstr_BCM_Send_info.ID );
					/*change the state so that the dispatcher don't be executed till the next ISR*/
					gstr_BCM_Send_info.State=TX_COMPLETE;
					/*change the state to send high byte*/
					gu8_SEND_SM=SEND_SIZE_HIGH;					
				}

				break;

				case SEND_SIZE_HIGH:
				if (gstr_BCM_Send_info.State==TX)
				{
					/*shifting the data to send the high byte*/
					UART_SendByte( ((gstr_Buffer_Send_Info.Buffer_Size)>>(8)) );
					gstr_BCM_Send_info.State=TX_COMPLETE;
					gu8_SEND_SM=SEND_SIZE_LOW;				
				}


				break;
				
				case SEND_SIZE_LOW:
				if (gstr_BCM_Send_info.State==TX)
				{
					/*mask the high byte and send the low byte*/
					UART_SendByte( (gstr_Buffer_Send_Info.Buffer_Size) );
					gstr_BCM_Send_info.State=TX_COMPLETE;
					gu8_SEND_SM=SEND_DATA;				
				}

				break;
				
				case SEND_DATA:
				/*check that there is more bytes to be sent*/
				if ( (gstr_BCM_Send_info.Index) < (gstr_Buffer_Send_Info.Buffer_Size) )
				{
	
					if (gstr_BCM_Send_info.State==TX)
					{
						UART_SendByte(gstr_Buffer_Send_Info.Data[gstr_BCM_Send_info.Index]);
						/*accumulate the check sum*/
						gstr_BCM_Send_info.Check_Sum+=(gstr_Buffer_Send_Info.Data[gstr_BCM_Send_info.Index]);				
						/*updates the index*/
						gstr_BCM_Send_info.Index++;
						gstr_BCM_Send_info.State=TX_COMPLETE;
						gu8_SEND_SM=SEND_DATA;
					}
				}
				
				else
				{
					/*go to the next state*/
					gu8_SEND_SM=SEND_CHECK_SUM;
				}
				break;
				
				case SEND_CHECK_SUM:
				
				if (gstr_BCM_Send_info.State==TX)
				{
					/*send the check sum*/				
					UART_SendByte ((gstr_BCM_Send_info.Check_Sum));
					/*my check that the check sum has been sent*/
					gstr_Buffer_Send_Info.ptrfu_BCM_Cbk();
					/*close the session*/
					gu8_SEND_SM=IDLE;
					/*free the channel*/
					gstr_BCM_Send_info.Buffer_State=UNLOCKED;
					break;				
				}

			}
		}
	}
	return BCM_TxDispatcher_Error;
	
}

/**************************************************************************
 * Function 	: BCM_RxDispatcher                                        *
 * Input 		: Icu_cfg : void								          *
 * Return 		: value of type ERROR_STATUS							  *			  
 * Description  : receives a byte each ISR								  *
 **************************************************************************/

ERR_STATUS BCM_RxDispatcher()
{
	ERR_STATUS BCM_RxDispatcher_Error=ERR_SUCCESS;
	
	/*checks if the BCM initialized or not*/
	if (gu8_BCM_Init_Status==INITIALIZED)
	{
		/*checks if the buffer is locked or not*/
		if (gstr_BCM_Receive_info.Buffer_State==LOCKED)
		{
			switch (gu8_RECEIVE_SM)
			{
				case RECEIVE_ID:
				if (gstr_BCM_Receive_info.State==RX)
				{
					UART_ReceiveByte( &gstr_BCM_Receive_info.ID );
					/*validate that it's the same ID or not for both Transmitter and receiver*/
					if (gu8_Receive_ID ==gstr_BCM_Receive_info.ID)
					{
						gstr_BCM_Receive_info.State=RX_COMPLETE;
						gu8_RECEIVE_SM=RECEIVE_SIZE_HIGH;
					}
					/*if not equal the session wont be RX so the program wont go to the next state*/
					else if (gu8_Receive_ID !=gstr_BCM_Receive_info.ID)
					{
						BCM_RxDispatcher_Error=ERR_RESOURCE_NOT_FOUND;
					}
					
					else 
					{
						/*DO NOTHING*/
					}
					
				}
				break;
				
				
				case RECEIVE_SIZE_HIGH:
				if (gstr_BCM_Receive_info.State==RX)
				{
					/*load the high byte by shifting the data by 8bits*/
					UART_ReceiveByte(&gu16_Buffer_Receive_Size);
					gu16_Buffer_Receive_Size=(gu16_Buffer_Receive_Size<<8);
					gstr_BCM_Receive_info.State=RX_COMPLETE;
					gu8_RECEIVE_SM=RECEIVE_SIZE_LOW;
				}
				break;
				
				
				case RECEIVE_SIZE_LOW:
				if (gstr_BCM_Receive_info.State==RX)
				{
					/*load the low byte*/
					UART_ReceiveByte(&gu16_Buffer_Receive_Size);
					gstr_BCM_Receive_info.State=RX_COMPLETE;
					gu8_RECEIVE_SM=RECEIVE_DATA;
				}

				break;
				
				case RECEIVE_DATA:
				/*as long as the index is less than the size, keep scan new elements*/
				if ( (gstr_BCM_Receive_info.Index) < (gu16_Buffer_Receive_Size) )
				{
					if (gstr_BCM_Receive_info.State==RX)
					{
						UART_ReceiveByte(&(gstr_Buffer_Receive_Info.Data));
						/*accumulate the check sum*/
						gua_Received_CheckSum+=gstr_Buffer_Receive_Info.Data;
						/*update the index to load the new element*/
						gstr_BCM_Receive_info.Index++;
						/*close the session*/
						gstr_BCM_Receive_info.State=RX_COMPLETE;
						gu8_RECEIVE_SM=RECEIVE_DATA;
					}
				}
				
				/*if the full buffer is loaded, go to the check sum state*/
				else
				{
					gu8_RECEIVE_SM=RECEIVE_CHECK_SUM;
				}
				break;
				
				case RECEIVE_CHECK_SUM:
				
				if (gstr_BCM_Receive_info.State==RX)
				{
					UART_ReceiveByte (&(gstr_BCM_Receive_info.Check_Sum));
					/*check that the data has been sent correctly*/
					if (gua_Received_CheckSum==gstr_BCM_Receive_info.Check_Sum)
						{
							/*my check for the code*/
							gstr_Buffer_Receive_Info.ptrfu_BCM_Cbk();
						}
						/*end the session*/
						gu8_RECEIVE_SM=IDLE;	
				}
				break;
			}
		}	
		else if (gu8_BCM_Init_Status==NOTINITIALIZED)
		{
			BCM_RxDispatcher_Error=NOTINITIALIZED;
		}
		
		else
		{
			/*DO NOTHING*/
		}		
		
	}
	else if (gstr_BCM_Receive_info.Buffer_State==LOCKED)
	{
		BCM_RxDispatcher_Error=ERR_INVALID_ARGU;
	}
	
	else 
	{
		/*DO NOTHING*/
	}
	return BCM_RxDispatcher_Error;
}


/**************************************************************************
 * Function 	: BCM_RxUnlock						                      *
 * Input 		: Icu_cfg : void								          *
 * Return 		: value of type ERROR_STATUS							  *			  
 * Description  : unlock the receiver session							  *
 **************************************************************************/

ERR_STATUS BCM_RxUnlock()
{
	ERR_STATUS BCM_RxUnlock_Error=ERR_SUCCESS;
	
	if (gstr_BCM_Receive_info.Buffer_State==LOCKED)
	{
		gstr_BCM_Receive_info.Buffer_State=UNLOCKED;
	}
	
	else if (gstr_BCM_Receive_info.Buffer_State==UNLOCKED)
	{
		BCM_RxUnlock_Error=ERR_INVALID_ARGU;
	}
	
	else 
	{
		/*DO NOTHING*/
	}
	
	return BCM_RxUnlock_Error;
	
}
