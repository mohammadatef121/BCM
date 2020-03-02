/*
 * TMU.h
 *
 * Created: 1/12/2020 1:05:13 PM
 *  Author: Johnny
 */ 


#ifndef TMU_H_
#define TMU_H_

/********************************************************/
/*					 INCLUDES      						*/
/********************************************************/

#include "std_types.h"
#include "common_retval.h"

/********************************************************/
/*					 Macros        						*/
/********************************************************/

#define ONE_SHOT    (1)
#define PERIODIC    (0)

/********************************************************/
/*					Structures    						*/
/********************************************************/

typedef struct {
	uint8 Repetition;
	uint8 current_time;
	uint8 Period_Time;
	void (*ptrfu_Timer_Cbk)(void);
}TMU_Task_cfg_t;



/**************************************************************************
 * Function 	: TMU_Init                                                *
 * Input 		: void											          *
 * Return 		: value of type ERR_STATUS							  *	
 * Description  : initialize the TMU by initializing the Timer with the	  *
				  configuration scanned from the user (timer 0,1,2)	      *
 **************************************************************************/

ERR_STATUS TMU_Init (void);
/**************************************************************************
 * Function 	: TMU_Start                                               *
 * Input 		: struct										          *
 * Return 		: value of type ERR_STATUS								  *	
 * Description  : starts the timer if it's the first time to load in	  *
				  the database, and load the tasks in the database	      *
 **************************************************************************/

ERR_STATUS TMU_Start(TMU_Task_cfg_t *gstr_TaskHandler);
/**************************************************************************
 * Function 	: TMU_Dispatcher                                          *
 * Input 		: void											          *
 * Return 		: value of type ERR_STATUS								  *	
 * Description  : updates the time of the system every "resolution" and	  *
				  takes the corresponding actions						  *
 **************************************************************************/
ERR_STATUS TMU_Dispatcher(void);
/**************************************************************************
 * Function 	: TMU_Stop                                                *
 * Input 		: pointer to function							          *
 * Return 		: value of type ERR_STATUS								  *	
 * Description  : stops specific task								      *
 **************************************************************************/
ERR_STATUS Tmu_Stop(void (*Func_ptr)(void));
/**************************************************************************
 * Function 	: TMU_DeInit                                              *
 * Input 		: void											          *
 * Return 		: value of type ERR_STATUS								  *	
 * Description  : deinitialize the whole TMU						      *
 **************************************************************************/
ERR_STATUS TMU_DeInit (void);



#endif /* TMU_H_ */