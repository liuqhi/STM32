
#include "motor.h"
#include <stdlib.h>

static uint8_t Motor_SetStatus(motor_t* motor, uint8_t status);

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
motor_t* Motor_Creat(void)
{
	motor_t* motor = NULL;
	
	motor = (motor_t*)malloc(sizeof(motor_t));
	
	motor->status = RESET;
	
	motor->SetStatus = Motor_SetStatus;
	
	return motor;
}

/**
	*status_0, status_1: MOTOR_RunState
*/
/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
static uint8_t Motor_SetStatus(motor_t* motor, uint8_t status)
{
	motor->status = status;
	
		/* void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) */
	switch(status)
	{
		case MOTOR_STOP:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
			break;
		case MOTOR_EXTEND:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
			break;
		case MOTOR_PULL_BACK:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
			break;
	}
	return 0;
}






























