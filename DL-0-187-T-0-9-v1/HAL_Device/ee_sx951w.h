
#ifndef __EE_SX951W_H
#define __EE_SX951W_H

#include "stm32f1xx_hal.h"


typedef enum
{
	EE_SX951W_UNLOCK = 0x00,
	
	EE_SX951W_LOCK = 0x01

} EE_SX951W_Status;



typedef struct __ee_sx951w_t ee_sx951w_t;

typedef struct __ee_sx951w_t
{
	
	uint8_t status;
	
	uint8_t (*GetStatus)(ee_sx951w_t*, uint8_t*);
	
} ee_sx951w_t;

ee_sx951w_t* EE_SX951W_Creat(void);

void EE_SX951W_Destroy(ee_sx951w_t* ee_sx951w);

#endif





















