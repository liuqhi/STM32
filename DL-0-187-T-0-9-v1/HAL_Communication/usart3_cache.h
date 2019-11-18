
#ifndef __USART3_CACHE_H
#define __USART3_CACHE_H

#include "stm32f1xx_hal.h"

#define USART3_CACHE_LEN 32

typedef struct _usart3_cache_t usart3_cache_t;
	
typedef struct _usart3_cache_t
{
	int8_t (*Usart3_Cache_Read)(usart3_cache_t*, uint8_t*);
	
	int8_t (*Usart3_Cache_Send)(usart3_cache_t*, uint8_t*, uint8_t);
	
} usart3_cache_t;

usart3_cache_t *Usart3_Cache_Creat(void);

void Usart3_Cache_Destroy(usart3_cache_t* usart3_cache);


#endif











