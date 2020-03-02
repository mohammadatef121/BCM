/*
 * TMU_lcfg.h
 *
 * Created: 1/12/2020 3:07:11 PM
 *  Author: Johnny
 */ 


#ifndef TMU_LCFG_H_
#define TMU_LCFG_H_

/********************************************************/
/*					 INCLUDES      						*/
/********************************************************/

#include "std_types.h"

/********************************************************/
/*					 Structures    						*/
/********************************************************/

 typedef struct 
 {
	 uint8 Timer_Channel;
	 uint16 Resolution;
	 }TMU_cfg_t;
	 
/********************************************************/
/*					 Extern      						*/
/********************************************************/	 

extern TMU_cfg_t gstr_TMU_Cfgs;

#endif /* TMU_LCFG_H_ */