
#ifndef __E3Z_LS61_H
#define __E3Z_LS61_H

#include "stm32f1xx_hal.h"


typedef struct __e3z_ls61_t e3z_ls61_t;

typedef struct __e3z_ls61_t
{
	uint8_t status;
	
	uint8_t (*Get_Status)(e3z_ls61_t*, uint8_t*);
	
} e3z_ls61_t;

e3z_ls61_t* E3Z_LS61_Creat(void);

void E3Z_LS61_Destroy(e3z_ls61_t* e3z_ls61);


#endif





















