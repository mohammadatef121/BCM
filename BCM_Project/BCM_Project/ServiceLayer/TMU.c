/*
 * TMU.c
 *
 * Created: 1/12/2020 1:05:00 PM
 *  Author: Johnny
 */ 

/********************************************************/
/*					 INCLUDES      						*/
/********************************************************/

#include "TMU.h"
#include "TMU_cfg.h"
#include "TMU_lcfg.h"
#include "DIO.h"
#include "Timer.h"

/********************************************************/
/*					 Macros        						*/
/********************************************************/

#define TCNT_1MS    (16UL)
#define OFF			(0)
#define ON			(1)

#define NOT_INITIALIZED (0)
#define INITIALIZED		(1)

#define MaxResolutionForTimer1 (4192)
#define MaxResolutionforTimer0or2 (16)

#define Timer0_channel 0
#define Timer1_channel 1
#define Timer2_channel 2

/********************************************************/
/*					Variables    						*/
/********************************************************/

uint8 gu8_index=0;
volatile uint8 gu8_Session=0;
uint8 gu8_Iteration=0;
/*the data base holding all of the tasks (array of structs)*/
TMU_Task_cfg_t gastr_TMU_TasksDataBase[MaxNumberOfTasks]; 

ERR_STATUS gs8_TMU_Init_State=NOT_INITIALIZED;

/********************************************************/
/*					CallBackFunctions    				*/
/********************************************************/

void gfu_TimerFlag_CBK (void)
{
	if (gu8_Session==OFF)
	{	/*enable the session for the Dispatcher*/
		gu8_Session = ON;
		/*preload the timer to count only the corresponding ticks not from 0*/
		Timer_Start(gstr_TMU_Cfgs.Timer_Channel,gstr_TMU_Cfgs.Resolution*TCNT_1MS);		
	}
}

/**************************************************************************
 * Function 	: TMU_Init                                                *
 * Input 		: void											          *
 * Return 		: value of type ERR_STATUS							  *	
 * Description  : initialize the TMU by initializing the Timer with the	  *
				  configuration scanned from the user (timer 0,1,2)	      *
 **************************************************************************/
ERR_STATUS TMU_Init (void)
{
	ERR_STATUS TMU_Init_Error;
	TMU_Init_Error=ERR_SUCCESS;
	
	if (gs8_TMU_Init_State==NOT_INITIALIZED)
	{
		/*initialize the timer*/
		Timer_cfg_s TMU;
		TMU.Timer_Cbk_ptr = gfu_TimerFlag_CBK;
		TMU.Timer_CH_NO=gstr_TMU_Cfgs.Timer_Channel;
		TMU.Timer_Mode=TIMER_MODE;
		TMU.Timer_Polling_Or_Interrupt=TIMER_INTERRUPT_MODE;
		TMU.Timer_Prescaler=TIMER_PRESCALER_1024;
		Timer_Init(&TMU);	
		gs8_TMU_Init_State=INITIALIZED;
	}
	else if (gs8_TMU_Init_State==INITIALIZED)
	{
		TMU_Init_Error=ERR_ALREADY_INITIALIZED;
	}
	return TMU_Init_Error;
	
}

/**************************************************************************
 * Function 	: TMU_Start                                               *
 * Input 		: struct										          *
 * Return 		: value of type ERR_STATUS								  *	
 * Description  : starts the timer if it's the first time to load in	  *
				  the database, and load the tasks in the database	      *
 **************************************************************************/

ERR_STATUS TMU_Start(TMU_Task_cfg_t *gstr_NewTask)
{	
	ERR_STATUS Error_MTU_Start=ERR_SUCCESS;
	
	if (gs8_TMU_Init_State==INITIALIZED)
	{
		/*check if it's the first element in the database*/
		if (gu8_index==0)
		{
			/*if the user input invalid channel number*/
			if (gstr_TMU_Cfgs.Timer_Channel>2)
			{
				Error_MTU_Start=ERR_INVALID_ARGU;
			}
			
			else 
			{	
				/*if the user input invalid resolution for timer 0 or timer 2*/
				if (((gstr_TMU_Cfgs.Timer_Channel==Timer0_channel)||(gstr_TMU_Cfgs.Timer_Channel==Timer0_channel)) && ((gstr_TMU_Cfgs.Resolution)>MaxResolutionforTimer0or2))
				{
					Error_MTU_Start=ERR_HW_CONSTRAIN;
				}
				/*if the user input invalid resolution for timer 1*/
				else if ((gstr_TMU_Cfgs.Timer_Channel==Timer1_channel) && (gstr_TMU_Cfgs.Resolution>MaxResolutionForTimer1))
				{
					Error_MTU_Start=ERR_HW_CONSTRAIN;
				}
				
				else 
				{
					Timer_Start(gstr_TMU_Cfgs.Timer_Channel,gstr_TMU_Cfgs.Resolution*TCNT_1MS);
				}
			}
		}
		
		/*check if the database has more spots free left*/
		if (gu8_index<MaxNumberOfTasks)
		{
			if (gstr_NewTask->ptrfu_Timer_Cbk==NULL)
			{
				Error_MTU_Start=ERR_NULL_PTR;
			}
			
			else if (gstr_NewTask->ptrfu_Timer_Cbk!=NULL)
			{
				gastr_TMU_TasksDataBase[gu8_index]=*gstr_NewTask;
				/*update the index*/
				gu8_index++;	
			}
		}		
	}
	else if (gs8_TMU_Init_State==NOT_INITIALIZED)
	{
		Error_MTU_Start=ERR_NOTINITIALIZED;
	}
	
	else 
	{
		/*DO NOTHING*/
	}
	return Error_MTU_Start;

}

/**************************************************************************
 * Function 	: TMU_Dispatcher                                          *
 * Input 		: void											          *
 * Return 		: value of type ERR_STATUS								  *	
 * Description  : updates the time of the system every "resolution" and	  *
				  takes the corresponding actions						  *
 **************************************************************************/

ERR_STATUS TMU_Dispatcher ()
{
	ERR_STATUS TMU_Dispatcher_Error=ERR_SUCCESS;
	
	if (gs8_TMU_Init_State==INITIALIZED)
	{
		/* to enter the dispatcher only after the ISR opens the session (every 1 resolutions)*/
		if(gu8_Session==ON)
		{
			/* loop all of the database*/
			for (gu8_Iteration=0;gu8_Iteration<=gu8_index;gu8_Iteration++)
			{
				/*update the timing of all tasks*/
				gastr_TMU_TasksDataBase[gu8_Iteration].current_time++;
				/*check if any function reached it's period*/
				if (gastr_TMU_TasksDataBase[gu8_Iteration].current_time==gastr_TMU_TasksDataBase[gu8_Iteration].Period_Time)
				{
					/*call the function which to be called every period*/
					if (gastr_TMU_TasksDataBase[gu8_Iteration].ptrfu_Timer_Cbk!=NULL)
					{
						gastr_TMU_TasksDataBase[gu8_Iteration].ptrfu_Timer_Cbk();
					}
					
					else if (gastr_TMU_TasksDataBase[gu8_Iteration].ptrfu_Timer_Cbk==NULL)
					{
						TMU_Dispatcher_Error=ERR_NULL_PTR;
					}
					
					else 
					{
						/*DO NOTHING*/
					}

					/*checks if the function is periodic or one shot*/
					if (gastr_TMU_TasksDataBase[gu8_Iteration].Repetition==PERIODIC)
					{
						gastr_TMU_TasksDataBase[gu8_Iteration].current_time=0;
					}
					else if (gastr_TMU_TasksDataBase[gu8_Iteration].Repetition==ONE_SHOT)
					{
						if (gastr_TMU_TasksDataBase[gu8_Iteration].ptrfu_Timer_Cbk!=NULL)
						{
							Tmu_Stop(gastr_TMU_TasksDataBase[gu8_Iteration].ptrfu_Timer_Cbk);
						}
						
						else if (gastr_TMU_TasksDataBase[gu8_Iteration].ptrfu_Timer_Cbk==NULL)
						{
							TMU_Dispatcher_Error=ERR_NULL_PTR;
						}
					}
				}
			}
			/*close the session*/
			gu8_Session=OFF;
		}		
	}
	
	else if (gs8_TMU_Init_State!=INITIALIZED)
	{
		TMU_Dispatcher_Error=ERR_NOTINITIALIZED;
	}
	
	else 
	{
		/*DO NOTHING*/
	}
	
	return TMU_Dispatcher_Error;

}

/**************************************************************************
 * Function 	: TMU_Stop                                                *
 * Input 		: pointer to function							          *
 * Return 		: value of type ERR_STATUS								  *	
 * Description  : stops specific task								      *
 **************************************************************************/

ERR_STATUS Tmu_Stop(void (*Func_ptr)(void))
{
	ERR_STATUS TMU_STOP_Error=ERR_SUCCESS;
	
	if (gs8_TMU_Init_State==INITIALIZED)
	{
		if (*Func_ptr!=NULL)
		{
			/*loop all of the database*/
			for (gu8_Iteration=0;gu8_Iteration<gu8_index;gu8_Iteration++)
			{
				/*determine the task of the function to be stopped*/
				if (gastr_TMU_TasksDataBase[gu8_Iteration].ptrfu_Timer_Cbk==Func_ptr)
				{
					/*over write the struct of the function to be stopped by the last struct in the database*/
					gastr_TMU_TasksDataBase[gu8_Iteration]=gastr_TMU_TasksDataBase[gu8_index];
					/*update the pointer of the database*/
					gu8_index--;
				}
			}			
		}
		else if (*Func_ptr==NULL)
		{
			TMU_STOP_Error=ERR_NULL_PTR;
		}
	}
	
	else if (gs8_TMU_Init_State!=INITIALIZED)
	{
		TMU_STOP_Error=NOT_INITIALIZED;
	}
	
	return TMU_STOP_Error;

}

/**************************************************************************
 * Function 	: TMU_DeInit                                              *
 * Input 		: void											          *
 * Return 		: value of type ERR_STATUS								  *	
 * Description  : deinitialize the whole TMU						      *
 **************************************************************************/

ERR_STATUS TMU_DeInit (void)
{
	ERR_STATUS TMU_DeInit_Error=ERR_SUCCESS;
	if (gs8_TMU_Init_State==INITIALIZED)
	{
		Timer_cfg_s TMU;
		/*made the callback function points to null, so the TMU wont reach the ISR, so the session will never be ON*/
		TMU.Timer_Cbk_ptr = NULL;
		TMU.Timer_CH_NO=gstr_TMU_Cfgs.Timer_Channel;
		TMU.Timer_Mode=TIMER_MODE;
		TMU.Timer_Polling_Or_Interrupt=TIMER_INTERRUPT_MODE;
		TMU.Timer_Prescaler=TIMER_PRESCALER_64;
		Timer_Init(&TMU);		
	}
	
	else if (gs8_TMU_Init_State==NOT_INITIALIZED)
	{
		TMU_DeInit_Error=ERR_NOTINITIALIZED;
	}
	
	return TMU_DeInit_Error;

}
