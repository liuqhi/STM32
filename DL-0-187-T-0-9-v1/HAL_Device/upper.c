/**
  ******************************************************************************
  * @file           : upper.c
  * @brief          : Upper program body
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
#include "upper.h"
#include "usart1_cache.h"
#include <stdlib.h>
#include <string.h>
#include "usart.h"

/* Private function prototypes -----------------------------------------------*/
static uint8_t Upper_Return_Status(upper_t* upper, uint8_t data[], uint8_t len);

static int8_t Upper_Write_To_Cache(upper_t* upper, uint8_t data);

static uint8_t Upper_Get_Command(upper_t* upper);

/**
  * @brief  Creat upper
  * @retval upper_t*
  */
upper_t* Upper_Creat(void)
{
	upper_t *upper = NULL;
	
	upper = (upper_t*)malloc(sizeof(upper_t));
	
	/** Initialize the upper parameter and funtion 
  */
	upper->host_infor.frame_head = HOST_FRAME_HEAD;	
	upper->host_infor.addr = HOST_ADDRESS;
	upper->host_infor.cmd = NULL_CMD;
	upper->host_infor.data_len = 0x01;
	upper->host_infor.data = 0;
	upper->host_infor.xor_check = 0;
	upper->host_infor.frame_tail = HOST_FRAME_TAIL;

	upper->slave_infor.frame_head = SLAVE_FRAME_HEAD;
	upper->slave_infor.addr = SLAVE_ADDRESS;
	upper->slave_infor.data_len = 0x01;
	upper->slave_infor.xor_check = 0x00;
	upper->slave_infor.frame_tail = SLAVE_FRAME_TAIL;
	
	upper->Get_Command = Upper_Get_Command;
	upper->Return_Status = Upper_Return_Status;
	Usart1_Set_Write_Handler(Upper_Write_To_Cache);
	
	return upper;
}

/**
  * @brief  Destroy upper
	* @param  upper: pointer to upper_t
  * @retval State 1: Sucess 0: Fail
  */
void Upper_Destroy(upper_t* upper)
{
	free(upper);
}

/**
  * @brief  Transmit data to the upper
	* @param  upper: Pointer to upper_t
	*					data:  Pointer data, equal to data[]
	*					len:	 The length of array data[]
  * @retval State 1: Sucess 0: Fail
  */
static uint8_t Upper_Return_Status(upper_t* upper, uint8_t data[], uint8_t len)
{
	uint8_t tx1_buffer[16];
	
	uint8_t xor_check = 0;
	
	
	/* Assign value to tx1_buffer */
	upper->slave_infor.data_len = len;
	
	tx1_buffer[0] = upper->slave_infor.frame_head;
	tx1_buffer[1] = upper->slave_infor.addr;
	tx1_buffer[2] = upper->host_infor.addr;
	tx1_buffer[3] = upper->slave_infor.cmd;
	tx1_buffer[4] = upper->slave_infor.data_len;
	
	for(uint8_t i = 0; i < len; i++)
	{
		tx1_buffer[5 + i] = data[i];
	}
	
	/* Get the XOR check value */
	for(uint8_t i = 0; i < (len + 4); i++)
	{
		xor_check ^= tx1_buffer[i + 1];
	}
	
	tx1_buffer[5 + len] = xor_check;
	upper->slave_infor.xor_check = xor_check;
	
	tx1_buffer[5 + len + 1] = SLAVE_FRAME_TAIL;
	
	/* Transmit tx_buffer to upper */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart1, tx1_buffer, 5 + len + 2, 0xFF);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	
	return 0;
}

/**
  * @brief  Acquire command frome upper and storing command to
	*					upper->host_infor.cmd
	* @param  upper: pointer to upper_t
  * @retval State 1: Sucess 0: Fail
  */
static uint8_t Upper_Get_Command(upper_t* upper)
{
	uint8_t xor_check = 0;
	
	for(uint8_t i = 1; i < 6; i++)
	{
		xor_check ^= 
			upper->frame_usart1[i];
	}
	
	if((xor_check == (upper->frame_usart1[6]))
			&& (upper->frame_usart1[7] == HOST_FRAME_TAIL))
	{
		upper->host_infor.cmd = upper->frame_usart1[3];
		upper->slave_infor.cmd = upper->host_infor.cmd;
		upper->host_infor.data = upper->frame_usart1[5];
		upper->host_infor.xor_check = xor_check;
	}
	
	switch(upper->slave_infor.cmd)
	{
		case PARTIAL_DISCHARGE_READ_CMD:
			upper->slave_infor.data_len = 0x04;
			break;
		default:
			upper->slave_infor.data_len = 0x01;
			break;
	}
	
	/* Clear the old upper command data and set all value to 0x00 */
	memset(upper->frame_usart1, 0x00, 16);
	
	return 0;
}

/**
  * @brief  Read Usart1 receive cache
	* @param  upper: pointer of upper_t
	*					data:  temporary variable to save data
  * @retval State 1: Sucess 0: Fail
  */
extern usart1_cache_t *p_usart1_cache;
#define USART1_CACHE_READ(data)\
while(p_usart1_cache->Usart1_Cache_Read(p_usart1_cache, data) == -1);

static int8_t Upper_Write_To_Cache(upper_t* upper, uint8_t data)
{
	/* Determine the frame head, host address and slave address */
	USART1_CACHE_READ(&data);
	
	if(data == HOST_FRAME_HEAD)
	{
		USART1_CACHE_READ(&data);
		
		if(data == HOST_ADDRESS)
		{
			USART1_CACHE_READ(&data);
			
			if(data == SLAVE_ADDRESS)
			{
				upper->frame_usart1[0] = 0xF2;
				upper->frame_usart1[1] = 0x01;
				upper->frame_usart1[2] = 0xA2;
				
				for(uint8_t i = 3; i < 8; i++)
					USART1_CACHE_READ(upper->frame_usart1 + i);
			}
		}
	}
	return 0;
}


/************************ (C) COPYRIGHT Dali-Tech *****END OF FILE*************/


















