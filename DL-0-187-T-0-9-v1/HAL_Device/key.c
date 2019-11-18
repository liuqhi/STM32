
#include <stdlib.h>
#include "key.h"

static uint8_t Key_GetStatus(key_t* key, uint8_t *status);

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
key_t* Key_Creat(void)
{
	key_t *key = NULL;
	
	key = (key_t*)malloc(sizeof(key_t));
	
	key->status = SET;
	
	key->Get_Status = Key_GetStatus;
	
	return key;
}


/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
static uint8_t Key_GetStatus(key_t* key, uint8_t *status)
{
	
	/* GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin); */
	static uint8_t key_up = 1;
	
	if(key_up && (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == 0))
	{
		HAL_Delay(10);
		key_up = 0;
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == RESET)
		{
			*status = SET;
			return SET;
		}
	}
	else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == SET)
	{
		key_up = 1;
		*status = RESET;
	}
	
	return RESET;
}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
void Key_Destroy(key_t* key)
{
	free(key);
}











