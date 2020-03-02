/*
 * BCM.h
 *
 * Created: 1/13/2020 8:32:08 PM
 *  Author: Johnny
 */ 

/********************************************************/
/*					Header Guards						*/
/********************************************************/

#ifndef BCM_H_
#define BCM_H_

/********************************************************/
/*					Includes							*/
/********************************************************/

#include "std_types.h"
#include "common_retval.h"
#include "BCM_cfg.h"


/**************************************************************************
 * Function 	: BCM_Init                                                *
 * Input 		: Icu_cfg : void								          *
 * Return 		: value of type ERROR_STATUS							  *			  
 * Description  : initialize the BCM by initializing the UART			  *
				  and make the initiate the struct by zeros and nulls	  *
 **************************************************************************/
ERR_STATUS BCM_Init();


/**************************************************************************
 * Function 	: BCM_Send                                                *
 * Input 		: Icu_cfg : void								          *
 * Return 		: value of type ERROR_STATUS							  *			  
 * Description  : create transmitting event by declare the size			  *	
				  and array of data and assign an ID for the session	  *
 **************************************************************************/
ERR_STATUS BCM_Send(uint8* array, uint16 Size,void (*Func_Send_ptr)(void));


/**************************************************************************
 * Function 	: BCM_Setup_Receive                                       *
 * Input 		: Icu_cfg : void								          *
 * Return 		: value of type ERROR_STATUS							  *			  
 * Description  : create receiving event by declare the size			  *	
				  and array of data and assign an ID for the session	  *
 **************************************************************************/
ERR_STATUS BCM_Setup_Receive(uint8* array, uint16 Size,void (*Func_Receive_ptr)(void));


/**************************************************************************
 * Function 	: BCM_TxDispatcher                                        *
 * Input 		: Icu_cfg : void								          *
 * Return 		: value of type ERROR_STATUS							  *			  
 * Description  : sends a byte each ISR									  *
 **************************************************************************/
ERR_STATUS BCM_RxDispatcher();


/**************************************************************************
 * Function 	: BCM_RxDispatcher                                        *
 * Input 		: Icu_cfg : void								          *
 * Return 		: value of type ERROR_STATUS							  *			  
 * Description  : receives a byte each ISR								  *
 **************************************************************************/
ERR_STATUS BCM_TxDispatcher();


/**************************************************************************
 * Function 	: BCM_RxUnlock						                      *
 * Input 		: Icu_cfg : void								          *
 * Return 		: value of type ERROR_STATUS							  *			  
 * Description  : unlock the receiver session							  *
 **************************************************************************/
ERR_STATUS BCM_RxUnlock();

#endif /* BCM_H_ */