/**
  ******************************************************************************
  * @file           : TEV.c
  * @brief          : TEV program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 Dali Tech.
  * All rights reserved.</center></h2>
  *	You may obtain a copy of the License at:
  *                             www.dali-tech.com
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/	
#include "TEV.h"
#include "usart3_cache.h"
#include <stdlib.h>
#include "usart.h"

/* Private function prototypes -----------------------------------------------*/
static uint8_t TEV_Set_Switch(TEV_t* TEV, uint8_t state);

static uint8_t TEV_Get_TEV_Data(TEV_t* TEV, int16_t *tev_data);

static uint8_t TEV_Send_Instruction_And_Get_Data(TEV_t* TEV, uint8_t instruction);

static int8_t TEV_Write_To_Cache(TEV_t* TEV, uint8_t data);

/**
  * @brief  Creat TEV
  * @retval TEV_t*
  */
TEV_t* TEV_Creat(void)
{
	TEV_t *TEV = NULL;
	
	TEV = (TEV_t*)malloc(sizeof(TEV_t));
	
	/** Initializes the upper parameter and funtion 
  */
	TEV->TEV_status = TEV_OFF;
	
	TEV->TEV_info.address = 0x00;
	
	TEV->TEV_info.baudrate = 0x03;		/* Baudrate: 115200 */
	
	TEV->TEV_info.running_mode = TEV_MODE;
	
	TEV->TEV_info.tev_data[0] = 0;
	
	TEV->TEV_info.ultrasonic_data[0] = 0;
	
	TEV->Set_Switch = TEV_Set_Switch;
	
	TEV->Send_Instruction_And_Get_Data = TEV_Send_Instruction_And_Get_Data;
	
	Usart3_Set_Write_Handler(TEV_Write_To_Cache);
	
	return TEV;
}

/**
  * @brief  Destroy TEV
	* @param  TEV: pointer to TEV_t
  * @retval State 1: Sucess 0: Fail
  */
void TEV_Destroy(TEV_t* TEV)
{
	free(TEV);
}

static uint8_t TEV_Set_Switch(TEV_t* TEV, uint8_t TEV_state)
{
	TEV->TEV_status = TEV_state;
	
	switch(TEV_state)
	{
		case TEV_OFF:
			
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
			break;
		
		case TEV_ON:
			
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
			break;
	}
	return 0;
}

/**
  * @brief  Transmit data to the TEV
	* @param  upper: Pointer to TEV_t
	*					instruction: Control TEV running status
  * @retval State 1: Sucess 0: Fail
  */
static uint8_t TEV_Send_Instruction_And_Get_Data(TEV_t* TEV, uint8_t instruction)
{
	int16_t data = 0;
	
	TEV->TEV_info.running_mode = instruction;
	
	switch(TEV->TEV_info.running_mode)
	{
		case TEV_MODE:
			
			HAL_UART_Transmit(&huart3, (uint8_t*)"R", 1, 0xFF);
			HAL_Delay(500);
			HAL_UART_Transmit(&huart3, (uint8_t*)&instruction, 1, 0xFF);
			HAL_Delay(500);
		
			for(uint8_t i = 0; i < 5; i++) 
			{
				HAL_UART_Transmit(&huart3, (uint8_t*)"G", 1, 0xFF);
				HAL_Delay(500);
				TEV_Get_TEV_Data(TEV, &data);
			}
			
			break;
			
		case ULTRASONIC_MODE:
			
			HAL_UART_Transmit(&huart3, (uint8_t*)"R", 1, 0xFF);
			HAL_Delay(500);
			HAL_UART_Transmit(&huart3, (uint8_t*)&instruction, 1, 0xFF);
			HAL_Delay(500);
		
			for(uint8_t i = 0; i < 5; i++) 
			{
				HAL_UART_Transmit(&huart3, (uint8_t*)"G", 1, 0xFF);
				HAL_Delay(500);
				TEV_Get_TEV_Data(TEV, &data);
			}
			
			break;
	}	
	return 0;
}

/**
  * @brief  Get data from TEV
	* @param  upper: Pointer to TEV_t
	*					tev_data:  Pointer data, equal to data[]
  * @retval State 1: Sucess 0: Fail
  */
static uint8_t TEV_Get_TEV_Data(TEV_t* TEV, int16_t *tev_data)
{
	static uint8_t count_1;
	static uint8_t count_2;
	
	uint8_t sum = 0;
	
	if(count_1 >= 5)	count_1 = 0;
	if(count_2 >= 5)	count_2 = 0;
	
	for(uint8_t i = 4; i < 6; i++)
	{
		sum += TEV->frame_usart3[i];
	}
	
	if(sum == TEV->frame_usart3[6])
	{
		/* Adaptale to new version(G300) and compatible older version(A200) */
		if(((TEV->frame_usart3[7] == 0x0D) && (TEV->frame_usart3[8] == 0x0A)) ||
			 ((TEV->frame_usart3[7] == 0x0A) && (TEV->frame_usart3[8] == 0x0D)))
		{
			switch(TEV->TEV_info.running_mode)
			{
				case TEV_MODE:
					
					*tev_data = (TEV->frame_usart3[4] << 8) | TEV->frame_usart3[5];
					TEV->TEV_info.tev_data[count_1++] = *tev_data;
				
					break;
				case ULTRASONIC_MODE:
					
					*tev_data = (TEV->frame_usart3[4] << 8) | TEV->frame_usart3[5];
					TEV->TEV_info.ultrasonic_data[count_2++] = *tev_data;
				
					break;
			}
			return 1;
		}
	}
	return 0;
}


/**
  * @brief  Read Uart3 receive cache
	* @param  upper: pointer of TEV_t
	*					data:  temporary variable to save data
  * @retval State 1: Sucess 0: Fail
  */
extern usart3_cache_t *p_usart3_cache;
#define USART3_CACHE_READ(data)\
while(p_usart3_cache->Usart3_Cache_Read(p_usart3_cache, data) == -1);
static int8_t TEV_Write_To_Cache(TEV_t* TEV, uint8_t data)
{
	/* Determine the frame head, TEV address s */
	USART3_CACHE_READ(&data)
	
	if(data == 0x52)
	{
		USART3_CACHE_READ(&data)
		
		if(data == 0x54)
		{
			USART3_CACHE_READ(&data)
			
			if(data == 0x01)
			{
				USART3_CACHE_READ(&data)
				
				if(data == 0x02)
				{
					TEV->frame_usart3[0] = 0x52;
					TEV->frame_usart3[1] = 0x54;
					TEV->frame_usart3[2] = 0x01;
					TEV->frame_usart3[3] = 0x02;
					for(uint8_t i = 4; i < 9; i++)
						USART3_CACHE_READ(TEV->frame_usart3 + i)
				}
			}
		}
	}
	return 0;
}


/************************ (C) COPYRIGHT Dali-Tech *****END OF FILE*************/


















