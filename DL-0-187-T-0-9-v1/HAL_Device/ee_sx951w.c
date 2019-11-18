

#include "ee_sx951w.h"
#include <stdlib.h>

static uint8_t EE_SX951W_GetStatus(ee_sx951w_t* ee_sx951w, uint8_t *status);


/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
ee_sx951w_t* EE_SX951W_Creat(void)
{
	ee_sx951w_t* ee_sx951w = NULL;
	
	ee_sx951w = (ee_sx951w_t*)malloc(sizeof(ee_sx951w_t));
	
	ee_sx951w->status = 1;
	
	ee_sx951w->GetStatus = EE_SX951W_GetStatus;
	
	return ee_sx951w;
	
}


/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
static uint8_t EE_SX951W_GetStatus(ee_sx951w_t* ee_sx951w, uint8_t *status)
{
	/* GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin); */
	*status = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12);
	
	ee_sx951w->status = *status;
	
	if(*status == 0) return RESET;
	else 						 return SET;
}


/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
void EE_SX951W_Destroy(ee_sx951w_t* ee_sx951w)
{
	free(ee_sx951w);
}










