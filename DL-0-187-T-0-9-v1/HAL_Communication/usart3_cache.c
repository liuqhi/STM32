
#include "usart3_cache.h"
#include "stm32f1xx_it.h"
#include <stdlib.h>

uint8_t usart3Cache[USART3_CACHE_LEN];

static uint8_t index_write = 0, index_read = 0;

static uint8_t is_looped = 0;

static int8_t Is_Cache_Empty(void);

static int8_t Is_Cache_Full(void);

static int8_t Usart3_Cache_Read(usart3_cache_t* usart3_cache, uint8_t *data);

static int8_t Usart3_Cache_Send(usart3_cache_t* usart3_cache, uint8_t *data, uint8_t len);

static int8_t Usart3_Cache_Write(uint32_t data);


usart3_cache_t *Usart3_Cache_Creat(void)
{
	usart3_cache_t *usart3_cache = NULL;
	
	usart3_cache = (usart3_cache_t*)malloc(sizeof(usart3_cache));
	
	usart3_cache->Usart3_Cache_Read = Usart3_Cache_Read;
	
	usart3_cache->Usart3_Cache_Send = Usart3_Cache_Send;
	
	Usart3_Set_Read_Handler(Usart3_Cache_Write);
	
	return usart3_cache;
}


void Usart3_Cache_Destroy(usart3_cache_t* usart3_cache)
{
	free(usart3_cache);
}

static int8_t Is_Cache_Empty(void)
{
	if((is_looped == 0) && (index_write == index_read))
		return 1;
	else
		return 0;
}

static int8_t Is_Cache_Full(void)
{
	if((is_looped == 1) && (index_write == index_read))
		return 1;
	else
		return 0;
}

static int8_t Usart3_Cache_Read(usart3_cache_t* usart3_cache, uint8_t *data)
{
	if(Is_Cache_Empty())
		return -1;
	else
	{
		*data = usart3Cache[index_read++];
		if(index_read == USART3_CACHE_LEN)
		{
			index_read = 0;
			is_looped = 0;
		}
		return 0;
	}
}

static int8_t Usart3_Cache_Send(usart3_cache_t* usart3_cache, uint8_t *data, uint8_t len)
{
	return 0;
}

static int8_t Usart3_Cache_Write(uint32_t data)
{
	if(Is_Cache_Full())
		return -1;
	else
	{
		usart3Cache[index_write++] = data;
		if(index_write == USART3_CACHE_LEN)
		{
			index_write = 0;
			is_looped = 1;
		}
		return 0;
	}
}



















