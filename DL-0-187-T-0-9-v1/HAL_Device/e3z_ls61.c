
#include <stdlib.h>
#include "e3z_ls61.h"


/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
static uint8_t E3Z_LS61_GetStatus(e3z_ls61_t* e3z_ls61, uint8_t *status);

e3z_ls61_t* E3Z_LS61_Creat(void)
{
	e3z_ls61_t* e3z_ls61 = NULL;
	
	e3z_ls61 = (e3z_ls61_t*)malloc(sizeof(e3z_ls61_t));
	
	e3z_ls61->status = RESET;
	
	e3z_ls61->Get_Status = E3Z_LS61_GetStatus;
	
	return e3z_ls61;
	
}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
static uint8_t E3Z_LS61_GetStatus(e3z_ls61_t* e3z_ls61, uint8_t *status)
{
	/* GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin); */
	*status = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2);
	
	e3z_ls61->status = *status;
	
	if(*status == 0)	return RESET;
	else							return SET;
}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
void E3Z_LS61_Destroy(e3z_ls61_t* e3z_ls61)
{
	free(e3z_ls61);
}










