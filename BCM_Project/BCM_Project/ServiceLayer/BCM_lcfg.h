/*
 * BCM_lcfg.h
 *
 * Created: 1/13/2020 8:32:26 PM
 *  Author: Johnny
 */ 


#ifndef BCM_LCFG_H_
#define BCM_LCFG_H_

#define UART		(0)
#define SPI			(1)

#define Transmitter		(0)
#define Receiver		(1)

#include "std_types.h"

typedef struct {
	uint8 MODE;
	uint8 Peripheral;
	}gstr_BCM_cfg_t;

extern gstr_BCM_cfg_t gstr_BCM_cfg;

#endif /* BCM_LCFG_H_ */