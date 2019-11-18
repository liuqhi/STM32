/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define EXPANSION_LINK_EXTEND 1
#define EXPANSION_LINK_RETURN 2
#define EXPANSION_LINK_STOP		3
#define KEY_STOP (HAL_GPIO_ReadPin(RESERVE_GPIO_Port, RESERVE_Pin))

	 
#define EE_SX951W_READY 0x01
#define EE_SX951W_NOT_READY 0x00
	 
typedef struct __SWITCH
{
	uint8_t ee_sx951w_switch;
	uint8_t e3z_ls61_switch;
	uint8_t key_value;
} SWITCH;	 
	 

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
extern SWITCH switch_state;
/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
void Hardware_Watchdog(void);
//void Expansion_Link_Control(uint8_t expan_link_cmd);
//uint8_t Back_Switch(void);
//uint8_t E3Z_LS61_Switch(void);
//uint8_t KEY_Scan(uint8_t mode);
void TEV_Power_Switch(uint8_t para);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
