/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "usart.h"
#include "gpio.h"
#include "control.h"

#include "key.h"
#include "e3z_ls61.h"
#include "ee_sx951w.h"
#include "motor.h"
#include "steering.h"
#include "usart2_cache.h"
#include "TEV.h"
#include "usart3_cache.h"
#include "upper.h"
#include "usart1_cache.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

key_t* key = NULL;

e3z_ls61_t* e3z_ls61 = NULL;

ee_sx951w_t* ee_sx951w = NULL;

usart2_cache_t *p_usart2_cache = NULL;

usart3_cache_t *p_usart3_cache = NULL;

usart1_cache_t *p_usart1_cache = NULL;

steering_t* steering = NULL;

motor_t* motor = NULL;

TEV_t* TEV = NULL;

upper_t* upper = NULL;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osThreadId myTask04Handle;
osThreadId myTask05Handle;
osThreadId myTask06Handle;
osThreadId myTask07Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

static int8_t(*Upper_Write)(upper_t*, uint8_t);

int8_t Usart1_Set_Write_Handler(int8_t (*Upper_Write_Handler)(upper_t*, uint8_t))
{
	Upper_Write = Upper_Write_Handler;
	
	return 0;
}

static int8_t(*Steering_Write)(steering_t*, uint8_t);

int8_t Usart2_Set_Write_Handler(int8_t (*Steering_Write_Cache_Handler)(steering_t*, uint8_t))
{
	Steering_Write = Steering_Write_Cache_Handler;
	
	return 0;
}

static int8_t(*TEV_Cache_Write)(TEV_t*, uint8_t);

int8_t Usart3_Set_Write_Handler(int8_t (*TEV_Cache_Write_Handler)(TEV_t*, uint8_t))
{
	TEV_Cache_Write = TEV_Cache_Write_Handler;
	
	return 0;
}
	
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);
void StartTask04(void const * argument);
void StartTask05(void const * argument);
void StartTask06(void const * argument);
void StartTask07(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 64);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityIdle, 0, 64);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, StartTask03, osPriorityIdle, 0, 64);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* definition and creation of myTask04 */
  osThreadDef(myTask04, StartTask04, osPriorityIdle, 0, 64);
  myTask04Handle = osThreadCreate(osThread(myTask04), NULL);

  /* definition and creation of myTask05 */
  osThreadDef(myTask05, StartTask05, osPriorityIdle, 0, 64);
  myTask05Handle = osThreadCreate(osThread(myTask05), NULL);

  /* definition and creation of myTask06 */
  osThreadDef(myTask06, StartTask06, osPriorityIdle, 0, 64);
  myTask06Handle = osThreadCreate(osThread(myTask06), NULL);

  /* definition and creation of myTask07 */
  osThreadDef(myTask07, StartTask07, osPriorityIdle, 0, 64);
  myTask07Handle = osThreadCreate(osThread(myTask07), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */

/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
	
	key = Key_Creat();
	
	e3z_ls61 = E3Z_LS61_Creat();
	
	ee_sx951w = EE_SX951W_Creat();
	
	p_usart2_cache = Usart2_Cache_Creat();
	
	p_usart3_cache = Usart3_Cache_Creat();
	
	p_usart1_cache = Usart1_Cache_Creat();
	
	steering = Steering_Creat();
	
	motor = Motor_Creat();
	
	TEV = TEV_Creat();
	
	upper = Upper_Creat();
	
	HAL_Delay(1000);
	
  /* Infinite loop */
  for(;;)
  {
		Hardware_Watchdog();	
		
		steering->Update_Status(steering);
		
		steering->Get_Status(steering);
		
		osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/


/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
	
	
	HAL_Delay(1500);
  /* Infinite loop */
  for(;;)
  {		
		System_Run(upper->host_infor.cmd);
//		motor->SetStatus(motor, MOTOR_PULL_BACK);
//		for(uint8_t i = 0U; i < 3U; i++)  steering->Set_Position(steering, 120);//SMSCL_Send_Instr();
//		upper->Return_Status(upper, test_frame, 1);
//		HAL_Delay(500);
		
    osDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
	
  /* Infinite loop */
	
  for(;;)
  {
		key->Get_Status(key, &(key->status));
		
		e3z_ls61->Get_Status(e3z_ls61, &(e3z_ls61->status));
		
		ee_sx951w->GetStatus(ee_sx951w, &(ee_sx951w->status));

    osDelay(1);
  }
  /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the myTask04 thread.
* @param argument: Not used
* @retval None
*/

/* USER CODE END Header_StartTask04 */
void StartTask04(void const * argument)
{
  /* USER CODE BEGIN StartTask04 */
	uint8_t data;
	
  /* Infinite loop */
  for(;;)
  {
		Upper_Write(upper, data);
		
		osDelay(1);
  }
  /* USER CODE END StartTask04 */
}

/* USER CODE BEGIN Header_StartTask05 */
/**
* @brief Function implementing the myTask05 thread.
* @param argument: Not used
* @retval None
*/


/* USER CODE END Header_StartTask05 */
void StartTask05(void const * argument)
{
  /* USER CODE BEGIN StartTask05 */

	uint8_t data;
	
  /* Infinite loop */
  for(;;)
  {
		Steering_Write(steering, data);
		
    osDelay(1);
  }
  /* USER CODE END StartTask05 */
}

/* USER CODE BEGIN Header_StartTask06 */
/**
* @brief Function implementing the myTask06 thread.
* @param argument: Not used
* @retval None
*/



/* USER CODE END Header_StartTask06 */
void StartTask06(void const * argument)
{
  /* USER CODE BEGIN StartTask06 */
	
	uint8_t data;

  /* Infinite loop */
  for(;;)
  {		
		TEV_Cache_Write(TEV, data);
		
    osDelay(1);
  }
  /* USER CODE END StartTask06 */
}

/* USER CODE BEGIN Header_StartTask07 */
/**
* @brief Function implementing the myTask07 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask07 */
void StartTask07(void const * argument)
{
  /* USER CODE BEGIN StartTask07 */

  /* Infinite loop */
  for(;;)
  {
		upper->Get_Command(upper);
		
    osDelay(1);
  }
  /* USER CODE END StartTask07 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */




/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
