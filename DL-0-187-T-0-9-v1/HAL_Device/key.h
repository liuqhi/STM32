


#ifndef __KEY_H
#define __KEY_H

#include "stm32f1xx_hal.h"

typedef struct __key_t key_t;

typedef struct __key_t
{
	uint8_t status;
	
	uint8_t (*Get_Status)(key_t*, uint8_t*);

} key_t;


key_t* Key_Creat(void);

void Key_Destroy(key_t* key);



#endif







