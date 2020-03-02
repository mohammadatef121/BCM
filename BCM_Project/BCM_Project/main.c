/*
 * main.c
 *
 *      Author: GABER
 */

/************************/
/*		                     INCLUDES	                                */
/************************/


#include "interrupts.h"
#include "BCM.h"
#include "DIO.h"
#include "util/delay.h"
#include "Sleep.h"
#include "TMU.h"

void a7a (void)
{
	PORTA_DIR|=0x40;
	PORTA_DATA|=0x40;
}


void main(void)
{
	void cbk1(void)
	{
		DIO_Toggle(GPIOA,BIT0);
	}

	void cbk2(void)
	{
		DIO_Toggle(GPIOA,BIT1);
	}

	void cbk3(void)
	{
		DIO_Toggle(GPIOA,BIT2);
	}
	
	TMU_Init();
		
	TMU_Task_cfg_t TMU0;
	TMU0.current_time=0;
	TMU0.Period_Time=1;
	TMU0.ptrfu_Timer_Cbk=cbk1;
	TMU0.Repetition=PERIODIC;
	TMU_Start(&TMU0);
		
	TMU_Task_cfg_t TMU2;
	TMU2.current_time=0;
	TMU2.Period_Time=1;
	TMU2.ptrfu_Timer_Cbk=cbk2;
	TMU2.Repetition=PERIODIC;
	TMU_Start(&TMU2);
		
	//TMU_Task_cfg_t TMU3;
	//TMU3.current_time=0;
	//TMU3.Period_Time=1;
	//TMU3.ptrfu_Timer_Cbk=cbk3;
	//TMU3.Repetition=PERIODIC;
	//TMU_Start(&TMU3);


	PORTA_DIR |= 0x40;
	BCM_Init();
	GIE();
	uint8 array[12]="MohammadAtef";
	BCM_Setup_Receive(array,12,a7a);
	Sleep_Init();
	PORTA_DIR|=0xFF;
	while(1)
	{
		PORTA_DATA|=0x80;
		TMU_Dispatcher();
		//BCM_TxDispatcher();
		BCM_RxDispatcher();
		PORTA_DATA&=~(0x80);
		Sleep_Start(Idle);
	}
}