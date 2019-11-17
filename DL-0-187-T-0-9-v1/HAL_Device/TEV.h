/**
  ******************************************************************************
  * @file           : TEV.h
  * @brief          : Header for TEV.c file.
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
#ifndef __TEV_H
#define __TEV_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Exported macro ------------------------------------------------------------*/
/** @defgroup TEV frame information
* @{
*/




/** @defgroup TEV state
  * @{
  */
typedef enum
{
	TEV_OFF = 0,														/*!< TEV off       						  */
	
	TEV_ON = 1															/*!< TEV on      			 				 	*/
	
} TEV_STATE;

/** @defgroup TEV running mode
  * @{
  */
typedef enum
{
	TEV_MODE = 'E',													/*!< TEV mode      					 		 */
	
	ULTRASONIC_MODE = 'A'										/*!< Ultrasonic mode   				   */
	
} TEV_RUNNING_MODE;


/**
  * @brief TEV_infor_t Structure definition
  */
typedef struct __TEV_info_t
{
	uint16_t software_versions;							/*!< Software version       		 */
	
	uint8_t address;												/*!< TEV address       					 */
																					/* Range: 1~123, default: 0      */
	
	uint8_t baudrate;												/*!< TEV baudrate       				 */	
			
	char running_mode;											/*!< TEV running mode         	 */
	
	int16_t tev_data[10];										/*!< TEV data         					 */		
		
	int16_t ultrasonic_data[10];						/*!< TEV data         					 */		
	
} TEV_info_t;

/**
  * @brief TEV_t Structure definition
  */
typedef struct __TEV_t TEV_t;

typedef struct __TEV_t
{
	uint8_t TEV_status;											/*!< TEV running mode         	 */
	
	TEV_info_t TEV_info;										/*!< TEV information          	 */
	
	uint8_t frame_usart3[16];								/*!< Hardware reveive cache      */		
	
	uint8_t (*Set_Switch)(TEV_t*, uint8_t);	/*!< TEV switch						       */	
	
																					/*!< Get TEV data					       */	
	uint8_t (*Send_Instruction_And_Get_Data)(TEV_t*, uint8_t);
	
} TEV_t;


/* Exported functions prototypes ---------------------------------------------*/
TEV_t* TEV_Creat(void);

void TEV_Destroy(TEV_t* TEV);

/* Register UART3 IRQHandler writing cache function */
int8_t Usart3_Set_Write_Handler(int8_t (*TEV_Cache_Write_Handler)(TEV_t*, uint8_t));


#ifdef __cplusplus
}
#endif

#endif	/* __TEV_H */



























