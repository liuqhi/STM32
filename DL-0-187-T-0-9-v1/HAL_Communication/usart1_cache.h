
#ifndef __USART1_CACHE_H
#define __USART1_CACHE_H

#include "stm32f1xx_hal.h"

#define USART1_CACHE_LEN 32

typedef struct _usart1_cache_t usart1_cache_t;

typedef struct _usart1_cache_t
{
	int8_t (*Usart1_Cache_Read)(usart1_cache_t*, uint8_t*);
	
	int8_t (*Usart1_Cache_Send)(usart1_cache_t*, uint8_t*, uint8_t);
	
} usart1_cache_t;

usart1_cache_t *Usart1_Cache_Creat(void);

void Usart1_Cache_Destroy(usart1_cache_t* usart1_cache);


#endif
