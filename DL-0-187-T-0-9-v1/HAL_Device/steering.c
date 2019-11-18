/**
  ******************************************************************************
  * @file           : steering.c
  * @brief          : Steering program body
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
#include "steering.h"
#include <stdlib.h>
#include "usart2_cache.h"
#include "usart.h"

/* Private function prototypes -----------------------------------------------*/
static uint8_t Steering_Set_Position(steering_t* steering, uint8_t angle);

static uint8_t Steering_Get_Status(steering_t* steering);

static uint8_t Steering_Update_Status(steering_t* steering);

static uint16_t Angle_To_uint16_t(uint8_t angle);

//static uint8_t uint16_t_To_Angle(uint16_t para);

static int8_t Steering_Write_To_Cache(steering_t* steering, uint8_t data);

/**
  * @brief	Creat steering 
  * @param  None          
  * @retval steering_t*
  */
steering_t* Steering_Creat(void)
{
	steering_t* steering = NULL;
	
	/** Initializes the steering parameter and funtion 
  */
	steering = (steering_t*)malloc(sizeof(steering_t));
	
	steering->position = 0;
	
	steering->Set_Position = Steering_Set_Position;
	
	steering->Get_Status = Steering_Get_Status;
	
	steering->Update_Status = Steering_Update_Status;
	
	Usart2_Set_Write_Handler(Steering_Write_To_Cache);
	
	return steering;
}

/**
  * @brief  Destroy steering
  * @param  steering: pointer to steering_t            
  * @retval None
  */
void Steering_Destroy(steering_t* steering)
{
	free(steering);
}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
static uint16_t Angle_To_uint16_t(uint8_t angle)
{
	float temp = .0f;
	
	temp = angle * (4095.f / 360.f);
	
	return (uint16_t)temp;
}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
//static uint8_t uint16_t_To_Angle(uint16_t para)
//{
//	float temp = .0f;
//	
//	temp = para * 360.f / 4095.f;
//	
//	return (uint8_t)temp;
//}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
static uint8_t Steering_Set_Position(steering_t* steering, uint8_t angle)
{
	uint8_t tx2_buffer[13], sum = 0;
	uint16_t temp = 0;

	steering->position = angle;
	temp = Angle_To_uint16_t(angle);
	
	tx2_buffer[0] = tx2_buffer[1] = 0xFF;
	tx2_buffer[2] = 0x01;
	tx2_buffer[3] = 0x09;
	tx2_buffer[4] = 0x03;
	tx2_buffer[5] = 0x2A;
	tx2_buffer[6] = temp & 0xFF;
	tx2_buffer[7] = (temp >> 8) & 0xFF;
	tx2_buffer[8] = 0x00;
	tx2_buffer[9] = 0x00;
	tx2_buffer[10] = 0x04;
	tx2_buffer[11] = 0x00;
	
	for(uint8_t i = 2; i < 12; i++)
	{
		sum += tx2_buffer[i];
	}
	
	tx2_buffer[12] = ~sum;
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart2, tx2_buffer, 13, 0xFF);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	
	return 0;
}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
static uint8_t Steering_Update_Status(steering_t* steering)
{
	uint8_t tx2_buffer[8], sum = 0;
	
	tx2_buffer[0] = tx2_buffer[1] = 0xFF;
	tx2_buffer[2] = 0x01;
	tx2_buffer[3] = 0x04;
	tx2_buffer[4] = 0x02;
	tx2_buffer[5] = 0x38;
	tx2_buffer[6] = 0x11;
	for(uint8_t i = 2; i < 7; i++)
	{
		sum += tx2_buffer[i];
	}
	tx2_buffer[7] = ~sum;
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart2, tx2_buffer, 8, 0xFF);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	
	return 0;
}

/**
  * @brief  
  * @param  
  *                
  * @retval 
  */

static uint8_t Steering_Get_Status(steering_t* steering)
{
	uint8_t sum = 0, temp;
	
	for(uint8_t i = 2; i < 22; i++)
	{
		sum += steering->frame_usart2[i];
	}
	
	temp = ~sum;
	
	if(temp == steering->frame_usart2[22])
	{
		steering->steering_infor.current_position = (steering->frame_usart2[6] << 8) | steering->frame_usart2[5];
		steering->steering_infor.current_speed = (steering->frame_usart2[8] << 8) | steering->frame_usart2[7];
		steering->steering_infor.current_load = (steering->frame_usart2[10] << 8) | steering->frame_usart2[9];
		steering->steering_infor.current_voltage = steering->frame_usart2[11];
		steering->steering_infor.current_temperature = steering->frame_usart2[12];
		steering->steering_infor.current_reg_write_flag = steering->frame_usart2[13];
		steering->steering_infor.moving_flag = steering->frame_usart2[15];
		steering->steering_infor.current_target_position = (steering->frame_usart2[17] << 8) | steering->frame_usart2[16];
		steering->steering_infor.now_current = (steering->frame_usart2[19] << 8) | steering->frame_usart2[18];
	}
	
	return 0;
}

extern usart2_cache_t *p_usart2_cache;
#define USART2_CACHE_READ(data) while(p_usart2_cache->Usart2_Cache_Read(p_usart2_cache, data) == -1);
static int8_t Steering_Write_To_Cache(steering_t* steering, uint8_t data)
{
	/* Is frame head 1 0xFF */
	USART2_CACHE_READ(&data)
	if(data == 0xFF)
	{
		/* Is frame head 2 0xFF */
		USART2_CACHE_READ(&data)
		if(data == 0xFF)
		{
			USART2_CACHE_READ(&data)
			if(data == 0x01)
			{
				steering->frame_usart2[0] = 0xFF;
				steering->frame_usart2[1] = 0xFF;
				steering->frame_usart2[2] = 0x01;
				USART2_CACHE_READ(&data)
				steering->frame_usart2[3] = 0x13;
				for(uint8_t i = 4; i < (0x13 + 4); i++)
					USART2_CACHE_READ(steering->frame_usart2 + i)
			}
		}
	}
	
	return 0;
}













