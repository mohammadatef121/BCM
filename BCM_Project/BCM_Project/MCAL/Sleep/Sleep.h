/*
 * Sleep.h
 *
 * Created: 1/12/2020 1:09:16 PM
 *  Author: Johnny
 */ 


#ifndef SLEEP_H_
#define SLEEP_H_

#include "std_types.h"

#define Idle				0x00
#define ADC_Noise_Reduction 0x01
#define Power_Down			0x02
#define Power_Save			0x03
#define Standby				0x06
#define Extended_Standby	0x07


ERROR_STATUS Sleep_Init();
ERROR_STATUS Sleep_Mode(uint8 Mode);


#endif /* SLEEP_H_ */