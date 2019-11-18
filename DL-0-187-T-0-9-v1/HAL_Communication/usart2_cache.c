
#include "usart2_cache.h"
#include "stm32f1xx_it.h"
#include <stdlib.h>

uint8_t usart2Cache[USART2_CACHE_LEN];

static uint8_t index_write = 0, index_read = 0;

static uint8_t is_looped = 0;

static int8_t Is_Cache_Empty(void);

static int8_t Is_Cache_Full(void);

static int8_t Usart2_Cache_Read(usart2_cache_t* usart2_cache, uint8_t *data);

static int8_t Usart2_Cache_Send(usart2_cache_t* usart2_cache, uint8_t *data, uint8_t len);

static int8_t Usart2_Cache_Write(uint32_t data);

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
usart2_cache_t* Usart2_Cache_Creat(void)
{
	usart2_cache_t *usart2_cache = NULL;
	
	usart2_cache = (usart2_cache_t*)malloc(sizeof(usart2_cache_t));
	
	usart2_cache->Usart2_Cache_Read = Usart2_Cache_Read;
	
	usart2_cache->Usart2_Cache_Send = Usart2_Cache_Send;
	
	Usart2_Set_Read_Handler(Usart2_Cache_Write);
	
	return usart2_cache;
}


/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
void Usart2_Cache_Destroy(usart2_cache_t* usart2_cache)
{
	free(usart2_cache);
}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
static int8_t Is_Cache_Empty(void)
{
	if((is_looped == 0) && (index_write == index_read))
		return 1;
	else
		return 0;
}


/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
static int8_t Is_Cache_Full(void)
{
	if((is_looped == 1) && (index_write == index_read))
		return 1;
	else
		return 0;
}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
static int8_t Usart2_Cache_Read(usart2_cache_t* usart2_cache, uint8_t *data)
{
	if(Is_Cache_Empty())
		return -1;
	else
	{
		*data = usart2Cache[index_read++];
		if(index_read == USART2_CACHE_LEN)
		{
			index_read = 0;
			is_looped = 0;
		}
		return 0;
	}
}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
int8_t Usart2_Cache_Send(usart2_cache_t* usart2_cache, uint8_t *data, uint8_t len)
{
	return 0;
}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
static int8_t Usart2_Cache_Write(uint32_t data)
{
	if(Is_Cache_Full())
		return -1;
	else
	{
		usart2Cache[index_write++] = data;
		if(index_write == USART2_CACHE_LEN)
		{
			index_write = 0;
			is_looped = 1;
		}
		return 0;
	}
}













