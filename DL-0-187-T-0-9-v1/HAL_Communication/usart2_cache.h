

#ifndef __USART2_CACHE_H
#define __USART2_CACHE_H

#include "stm32f1xx_hal.h"

#define USART2_CACHE_LEN 64

typedef struct _usart2_cache_t usart2_cache_t;
	
typedef struct _usart2_cache_t
{
	int8_t (*Usart2_Cache_Read)(usart2_cache_t*, uint8_t*);
	
	int8_t (*Usart2_Cache_Send)(usart2_cache_t*, uint8_t*, uint8_t);
	
} usart2_cache_t;

usart2_cache_t *Usart2_Cache_Creat(void);

void Usart2_Cache_Destroy(usart2_cache_t* usart2_cache);

#endif









