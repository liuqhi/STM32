
#include "usart1_cache.h"
#include "stm32f1xx_it.h"
#include <stdlib.h>

uint8_t usart1Cache[USART1_CACHE_LEN];

static uint8_t index_write = 0, index_read = 0;

static uint8_t is_looped = 0;

static int8_t Is_Cache_Empty(void);

static int8_t Is_Cache_Full(void);

static int8_t Usart1_Cache_Read(usart1_cache_t* usart1_cache, uint8_t *data);

static int8_t Usart1_Cache_Send(usart1_cache_t* usart1_cache, uint8_t *data, uint8_t len);

static int8_t Usart1_Cache_Write(uint32_t data);


/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
usart1_cache_t* Usart1_Cache_Creat(void)
{
	usart1_cache_t *usart1_cache = NULL;
	
	usart1_cache = (usart1_cache_t*)malloc(sizeof(usart1_cache_t));
	
	usart1_cache->Usart1_Cache_Read = Usart1_Cache_Read;
	
	usart1_cache->Usart1_Cache_Send = Usart1_Cache_Send;
	
	Usart1_Set_Read_Handler(Usart1_Cache_Write);
	
	return usart1_cache;
}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
void Usart1_Cache_Destroy(usart1_cache_t* usart1_cache)
{
	free(usart1_cache);
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
static int8_t Usart1_Cache_Read(usart1_cache_t* usart1_cache, uint8_t *data)
{
	if(Is_Cache_Empty())
		return -1;
	else
	{
		*data = usart1Cache[index_read++];
		if(index_read == USART1_CACHE_LEN)
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
int8_t Usart1_Cache_Send(usart1_cache_t* usart1_cache, uint8_t *data, uint8_t len)
{
	return 0;
}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
static int8_t Usart1_Cache_Write(uint32_t data)
{
	if(Is_Cache_Full())
		return -1;
	else
	{
		usart1Cache[index_write++] = data;
		if(index_write == USART1_CACHE_LEN)
		{
			index_write = 0;
			is_looped = 1;
		}
		return 0;
	}
}



















