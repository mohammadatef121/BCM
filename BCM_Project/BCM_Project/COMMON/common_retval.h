/******************************************************************************
 *
 * 	Module: 		common
 *
 *	File Name: 		common_retval.h
 *
 * 	Description: 	holds common errors returned over modules
 *
 * 	Authors: 		Mahmoud Saad
 * 	
 *	Created on:		Monday, 13-Jan-20 9:56:43 PM
 *
 *******************************************************************************/

#ifndef COMMON_RETVAL_H_
#define COMMON_RETVAL_H_

/************************************************************************/
/*		                        Defines                 		        */
/************************************************************************/
/* error types*/
#define ERR_STATUS 						sint8
#define ERR_SUCCESS 				    ( 0)
#define ERR_INVALID_ARGU	    	    (-1)
#define ERR_NOTINITIALIZED 				(-2)
#define ERR_ALREADY_INITIALIZED  		(-3)
#define ERR_NULL_PTR					(-4)
#define ERR_RESOURCE_NOT_FOUND 			(-5)
#define ERR_HW_CONSTRAIN				(-6)
#define ERR_BUFFER_FULL					(-7)
#define ERR_TIMER						(-8)

/* modules bases */
#define TMU_BASE						(-50)


#endif /* COMMON_RETVAL_H_ */
