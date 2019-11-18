

#ifndef __CONTROL_H
#define __CONTROL_H


#include "stm32f1xx_hal.h"

/* State */
#define SMSCL_BUSY 					 0x01U
#define SMSCL_OK					 	 0x02U
#define E3Z_LS61_ERROR 		   0x03U
#define E3Z_LS61_OK					 0x04U
#define KEY_STATE_ERROR			 0x05U



typedef enum
{
	Expansion_Link_Init 	 = 0x00U,
	Expansion_Link_Extend    = 0x01U,
	Expansion_Link_Shinkage	 = 0x02U,
	Expansion_Link_Stop 	 = 0x03U
} Expansion_Link_StatusTypeDef;


/* CMD */
void System_Run(uint8_t cmd);



#endif



