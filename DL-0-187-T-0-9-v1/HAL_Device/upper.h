/**
  ******************************************************************************
  * @file           : upper.h
  * @brief          : Header for upper.c file.
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
#ifndef __UPPER_H
#define __UPPER_H





#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Exported macro ------------------------------------------------------------*/
/** @defgroup Host frame information
* @{
*/ 
#define HOST_FRAME_HEAD 0xF2
#define HOST_ADDRESS 		0x01
#define HOST_FRAME_TAIL 0xF3


/** @defgroup Slaver frame information
* @{
*/ 
#define SLAVE_FRAME_HEAD 0xF2
#define SLAVE_ADDRESS		 0xA2
#define SLAVE_FRAME_TAIL 0xF3

/** @defgroup Upper_Command
  * @{
  */
typedef enum
{
	NULL_CMD = 												0x00,
	
	PARTIAL_DISCHARGE_AUTO_READ_CMD = 0x01,	/*!< Auto read partial discharge     
                                             Value is allowed for auto reading  
																						  partial discharge  */
	PARTIAL_DISCHARGE_READ_CMD = 			0x02,	/*!< Read partial discharge     
                                             Value is allowed for reading  
																						  partial discharge  */
	EXPAND_LINK_MANUAL_CONTROL_CMD = 	0x03,	/*!< EXPAND_LINK manual control    
                                             Value is allowed for Expand  
																						 link manual control */
	SMSCL_MANUAL_CONTROL_CMD = 				0x04,	/*!< SMSCL manual control   
                                             Value is allowed for SMSCL 
																						 manual control */	
	SMSCL_DEMARCATE_CMD =							0x05, /*!< Demarcate    
                                             Value is allowed for demarcate SMSCL */
	PARTIAL_DISCHARGE_READ_ONLY_CMD = 0x06  /*!< Read only   
                                             Value is allowed for read only */
} Upper_Command;

/**
  * @brief Host information Structure definition
  */
typedef struct __host_info_t
{
	uint8_t frame_head;											/*!< Host frame head       		 */
	
	uint8_t addr;														/*!< Host address              */
	
	uint8_t cmd;														/*!< Host command      				 */
	
	uint8_t data_len;												/*!< Host data length       	 */
	
	uint8_t data;														/*!< Host valid data		       */
	
	uint8_t xor_check;											/*!< Host XOR check byte       */
	
	uint8_t frame_tail;											/*!< Host frame tail       		 */
	
} host_infor_t;

/**
  * @brief Slaver information Structure definition
  */
typedef struct __slave_info_t
{
	uint8_t frame_head;											/*!< Slaver frame head       		 */
	
	uint8_t addr;														/*!< Slaver address     	  		 */
	
	uint8_t cmd;														/*!< Slaver command      				 */
	
	uint8_t data_len;												/*!< Slaver data length       	 */
	
	uint8_t data[4];												/*!< Slaver valid data		       */
	
	uint8_t xor_check;											/*!< Slaver XOR check byte       */
	
	uint8_t frame_tail;											/*!< Slaver frame tail       		 */
	
} slave_infor_t;

/**
  * @brief upper_t Structure definition
  */
typedef struct __upper_t upper_t;

typedef struct __upper_t
{
	host_infor_t host_infor;								/*!< Upper host information  			 */
	
	slave_infor_t slave_infor;							/*!< Upper slaver information  		 */
	
	uint8_t frame_usart1[16];								/*!< Upper receive frame   				 */
	
	uint8_t (*Get_Command)(upper_t*);				/*!< Pointer for command function  */
																																							
	uint8_t (*Return_Status)(upper_t*, 			/*!< Pointer for transmit status 	 */
													 uint8_t*, uint8_t);
																					
} upper_t;


/* Exported functions prototypes ---------------------------------------------*/
upper_t* Upper_Creat(void);

void Upper_Destroy(upper_t* upper);

/* Register USART1 IRQHandler writing cache function */
int8_t Usart1_Set_Write_Handler(int8_t (*Usart1_Write_Handler)(upper_t*, uint8_t));


#ifdef __cplusplus
}
#endif

#endif  /* __UPPER_H */


























