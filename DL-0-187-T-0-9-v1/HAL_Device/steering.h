/**
  ******************************************************************************
  * @file           : steering.h
  * @brief          : Header for steering.c file.
	*										This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STEERING_H
#define __STEERING_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"


/* Exported macro ------------------------------------------------------------*/
/** @defgroup Steering
* @{
*/



/** @defgroup Steering error state
  * @{
  */
typedef enum
{
	OVER_LOAD = 0,													/*!< Over load       			 					*/
	
	OVER_FLUENT = 1,												/*!< Over fluent     			 					*/
	
	OVER_HEATING = 2,												/*!< Over heating     							*/
	
	ANGLE_ERROR = 3,												/*!< Angle error	     		 					*/
	
	OVER_OR_LACK_VOLTAGE = 4								/*!< Over or land of voltage				*/
	
} Steering_Error;


/**
  * @brief Steeing Information Structure definition
  */
typedef struct __steering_infor_t
{
	uint16_t software_versions;							/*!< Software versions 		 					*/
	
	uint8_t id;															/*!< ID number, default: 0x01	 			*/

	uint8_t baudrate;												/*!< Buadrate, default :0x04)	 			*/

	uint8_t return_delay_time;							/*!< 	Return delay time,(0~254)*2us */
	
	uint8_t response_status_level;					/*!< 0: no responce, 1: responce	 	*/

	uint16_t min_angle_limit;								/*!< angle_limit,(0~4095)->(0~360)	*/
	uint16_t max_angle_limit;
	
	uint8_t max_temper_limit;								/*!< Max: 80 degree centigrade			*/

	uint8_t max_input_voltage;							/*!< Max: 140 -- 14 V								*/

	uint8_t min_intput_voltage;							/*!< Min: 60 -- 6 V 								*/

	uint16_t max_torque;										/*!< Max: 1000											*/
	

	uint8_t unload_condition;								/* bit7 bit6  bit5  bit4  bit3  bit2  bit1  bit0 */
	uint8_t LED_warning_condition;					/*  --   --  torque  --  fluent heat angle voltage  */

	uint8_t auto_control_P;									/*!< Max: 0x0F(15)									*/	
	
	uint8_t auto_control_D;									/*!< Max: 0x0F(15)									*/
	
	uint8_t auto_control_I;									/*!< Max: 0x0F(15)									*/
	
	uint16_t min_PWM;												/*!< maximum: 0x64(100)<<8|0x64(100) */

	uint8_t clockwise_intensi_zone;					/*!<Dead zone, 1: 0.087 degree 		 	*/
	uint8_t counterclockwise_intensi_zone;

	uint16_t integral_limit;								/*!< Default: 0x00 									*/
	
																		/* bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0 */
	uint8_t running_mode;											
	
	uint8_t protect_current;
	
	uint8_t torque_switch;
	
	uint16_t target_position;
	
	uint16_t running_time;
	
	uint16_t running_speed;
	
	uint8_t lock_sign;
	
	uint16_t current_position;
	
	uint16_t current_speed;
	
	uint16_t current_load;
	
	uint8_t current_voltage;
	
	uint8_t current_temperature;
	
	uint8_t current_reg_write_flag;
	
	uint8_t moving_flag;
	
	uint16_t current_target_position;
	
	uint16_t now_current;

} steering_infor_t;


/**
  * @brief steering_t Structure definition
  */
typedef struct __steering_t steering_t;

typedef struct __steering_t
{
	uint8_t position;												/*!< Set position     			 				*/
	
	steering_infor_t steering_infor;				/*!< Steering information		  			*/
	
	uint8_t frame_usart2[32];								/*!< Steering receive frame   			*/
	
																					/*!< Pointer for set position  			*/
	uint8_t (*Set_Position)(steering_t*, uint8_t); 
																					
	uint8_t (*Get_Status)(steering_t*);			/*!< Pointer for get status				  */
	
	uint8_t (*Update_Status)(steering_t*);	/*!< Pointer for update status		  */
	
} steering_t;

/* Exported functions prototypes ---------------------------------------------*/
steering_t* Steering_Creat(void);

void Steering_Destroy(steering_t* steering);

/* Register USART2 IRQHandler writing cache function */
int8_t Usart2_Set_Write_Handler(int8_t (*Steering_Write_Cache_Handler)\
															 (steering_t*, uint8_t));

#ifdef __cplusplus
}
#endif

#endif	/* __STEERING_H */





















