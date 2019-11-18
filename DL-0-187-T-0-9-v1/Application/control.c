
#include "control.h"
#include "gpio.h"
#include "usart.h"
#include "string.h"


#include "key.h"
#include "e3z_ls61.h"
#include "ee_sx951w.h"
#include "motor.h"
#include "steering.h"
#include "TEV.h"
#include "upper.h"

extern key_t* key;

extern e3z_ls61_t* e3z_ls61;

extern ee_sx951w_t* ee_sx951w ;

extern steering_t* steering;

extern motor_t* motor;

extern TEV_t* TEV;

extern upper_t* upper;


extern TIM_HandleTypeDef htim2;
uint8_t second_count;
uint8_t is_expansion_timeout;


Expansion_Link_StatusTypeDef expansion_now_status, expansion_pre_status;


//static Expansion_Link_StatusTypeDef Update_Expansion_Link_Status(void);

//static void Get_Excution_Command(void);

static uint8_t Partial_Discharge_AutoRead_State(void);
static uint8_t Partial_Discharge_AutoRead(void);
static uint8_t Expand_Link_Manual_Control(void);
static uint8_t SMSCL_Manual_Control(void);
static uint8_t Steering_Calibration(void);

static uint8_t Partial_Discharge_ReadOnly(void);

static uint8_t Test_Procedure(void);


/**
  * @brief  
  * @param  
  *                
  * @retval 
  */
void System_Run(uint8_t cmd)
{
	switch(cmd)
	{
		case 0x01:
			Partial_Discharge_AutoRead_State();
			break;
		case 0x02:
			Partial_Discharge_AutoRead();
			break;
		case 0x03:
			Expand_Link_Manual_Control();
			break;
		case 0x04:
			SMSCL_Manual_Control();
			break;
		case 0x05:
			Steering_Calibration();
			break;
		case 0x06:
			Partial_Discharge_ReadOnly();
			break;
		case 0x07:
			Test_Procedure();
			break;
		default:
			break;
	}
}


static uint8_t Partial_Discharge_AutoRead_State(void)
{
	if(ee_sx951w->status == EE_SX951W_UNLOCK)
	{
		/* Transmit the expansion link error state to upper */
		upper->slave_infor.data_len = 1;
		upper->slave_infor.cmd = 0x01;
		upper->slave_infor.data[0] = 0x03;
		
		upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
		
		/* Return to the initial posiotion. */
		motor->SetStatus(motor, MOTOR_PULL_BACK);
		HAL_Delay(10000);
		
		steering->Set_Position(steering, 0);
		HAL_Delay(10000);
		
		upper->host_infor.cmd = 0x02;
		
		return SMSCL_BUSY;
	}
	else
	{
		if(e3z_ls61->status == 0U)
		{
			/* Transmit the E3Z_LS61 error state to upper */
			upper->slave_infor.data_len = 1;
			upper->slave_infor.cmd = 0x01;
			upper->slave_infor.data[0] = 0x04;
		
			upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
		
			upper->host_infor.cmd = 0x00;
			
			return E3Z_LS61_ERROR;
		}
		else if(e3z_ls61->status == 1U)
		{
			/* Transmit the E3Z_LS61 ok state to upper */
			upper->slave_infor.data_len = 1;
			upper->slave_infor.cmd = 0x01;
			upper->slave_infor.data[0] = 0x01;
		
			upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
		
			upper->host_infor.cmd = 0x02;
			
			return E3Z_LS61_OK;
		}
	}
	
	return 0;
}



static uint8_t Partial_Discharge_AutoRead(void)
{
	/* Stop TIM2, clear the timeout second_count and clear the expansion link flag. */
	HAL_TIM_Base_Stop_IT(&htim2);
	second_count = 0;
	is_expansion_timeout = 0;
	
	/* Is the expansion link at the original position, if it is, excute the next step */
	/* if not, transmit the error state to the upper and return the error state.   */
	if(ee_sx951w->status == EE_SX951W_UNLOCK)
	{
		/* Return to the initial posiotion. */
		motor->SetStatus(motor, MOTOR_PULL_BACK);
		HAL_Delay(10000);
		
		steering->Set_Position(steering, 0);
		HAL_Delay(10000);
		
		/* Transmit the expansion link error state to upper */
		upper->slave_infor.data_len = 1;
		upper->slave_infor.cmd = 0x02;
		upper->slave_infor.data[0] = 0x03;
		
		upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
		
		/* Return excution error status. */
		return SMSCL_BUSY;
	}
	/* The expansion link an the init position, which can excute the next action. */
	else if(ee_sx951w->status == EE_SX951W_LOCK)
	{
		/* Determine whether the ditance is right. */
		/* If the distance switch is not right, transmit the error information to the upper. */
		if(e3z_ls61->status == 0U)
		{
			/* Transmit the error state to upper */
			upper->slave_infor.data_len = 1;
			upper->slave_infor.cmd = 0x02;
			upper->slave_infor.data[0] = 0x04;
		
			upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
		
			upper->host_infor.cmd = 0x00;
			/* Return excution error status. */
			return E3Z_LS61_ERROR;
		}
		/* If the distance switch is right, excute the main task. */
		else if(e3z_ls61->status == 1U)
		{
			/* Turn the steering gare to its intended position. */
			for(uint8_t i = 0; i < 3; i++)  steering->Set_Position(steering, 180);//SMSCL_Send_Instr1();
			/* When the steering runs from the initial position to the destination position, it needs */
			/* 10 seconds approximately. */
			HAL_Delay(12000);
			
			/* Detect if the steering goes back the initial position. */
			while(ee_sx951w->status != EE_SX951W_READY)
			{
				/* Check if the key is pressed. If pressed, stops stretching and start the detection. */
				/* If not pressed, streches the extension link until timeout value up to 20 seconds.  */
				/* Then shrinks the extension link. 																									*/
				if(key->status == 0U) 
				{
					/* Stop TIM2, clear the timeout second_count and clear the expansion link flag. */
					HAL_TIM_Base_Stop_IT(&htim2);
					second_count = 0;
					
					/* The key is pressed, stop the extension link immediately. */
					motor->SetStatus(motor, MOTOR_STOP);
					
					/* Turn on the partial discharge switch. */
					TEV->Set_Switch(TEV, TEV_ON);
					
					/* Wait for 10 seconds until the partial discharge working stability. */
					HAL_Delay(5000);

					/* Transmit the command of reading partial diacharge(TEV) data for 10 times. */
					TEV->Send_Instruction_And_Get_Data(TEV, TEV_MODE);
					
					upper->slave_infor.data_len = 4;
					upper->slave_infor.cmd = 0x02;
					upper->slave_infor.data[0] = (TEV->TEV_info.tev_data[2] >> 8) & 0xFF;
					upper->slave_infor.data[1] = TEV->TEV_info.tev_data[2] & 0xFF;
					upper->slave_infor.data[2] = (TEV->TEV_info.ultrasonic_data[2] >> 8) & 0xFF;
					upper->slave_infor.data[3] = TEV->TEV_info.ultrasonic_data[2] & 0xFF;
					
					upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
					
					/* Shrink the extension about 20 millimeter. */
					motor->SetStatus(motor, MOTOR_PULL_BACK);
					HAL_Delay(3000);
					/* Stop the expansion link to read ultrasonic data. */
					motor->SetStatus(motor, MOTOR_STOP);
				
					/* Transmit the command of reading partial diacharge(ultrasonic) data for 10 times. */
					TEV->Send_Instruction_And_Get_Data(TEV, ULTRASONIC_MODE);

					TEV->Set_Switch(TEV, TEV_OFF);
					
					/* Transmit TEV and ultrasonic data to the upper. */
					upper->slave_infor.data_len = 4;
					upper->slave_infor.cmd = 0x02;
					upper->slave_infor.data[0] = (TEV->TEV_info.tev_data[2] >> 8) & 0xFF;
					upper->slave_infor.data[1] = TEV->TEV_info.tev_data[2] & 0xFF;
					upper->slave_infor.data[2] = (TEV->TEV_info.ultrasonic_data[2] >> 8) & 0xFF;
					upper->slave_infor.data[3] = TEV->TEV_info.ultrasonic_data[2] & 0xFF;
					
					upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
	
					memset(TEV->TEV_info.tev_data, 0x00, sizeof(TEV->TEV_info.tev_data));
					memset(TEV->TEV_info.ultrasonic_data, 0x00, sizeof(TEV->TEV_info.ultrasonic_data));
					/* Shrink the expansion link to the end. */
					motor->SetStatus(motor, MOTOR_PULL_BACK);
					HAL_Delay(15000U);
					
					motor->SetStatus(motor, MOTOR_STOP);
					
					/* Transmit the SMSCL turn around 0 degree command to the initial position. */
					for(uint8_t i = 0U; i < 3U; i++)  steering->Set_Position(steering, 0);//SMSCL_Send_Instr();
					HAL_Delay(12000);
					
					upper->host_infor.cmd = 0x00;
				}
				else if(key->status == 1U)
				{
					HAL_TIM_Base_Start_IT(&htim2);
					if(second_count < 20U)
					{
//						Expansion_Link_Control(EXPANSION_LINK_EXTEND);
						motor->SetStatus(motor, MOTOR_EXTEND);
						is_expansion_timeout = 0U;
					}
					else
					{
						is_expansion_timeout = 1U;
					}
				}
				if(is_expansion_timeout == 1U)
				{
					HAL_TIM_Base_Stop_IT(&htim2);
					second_count = 0;
					
					upper->slave_infor.data_len = 1;
					upper->slave_infor.cmd = 0x02;
					upper->slave_infor.data[0] = 0x05;
					upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
					
					motor->SetStatus(motor, MOTOR_PULL_BACK);
					HAL_Delay(18000U);
					
					motor->SetStatus(motor, MOTOR_STOP);
					
					for(uint8_t i = 0U; i < 3U; i++)  steering->Set_Position(steering, 0);//SMSCL_Send_Instr();
					HAL_Delay(12000U);
					
					upper->host_infor.cmd = 0x00;
				}
			}
		}
	}
	
	upper->host_infor.cmd = 0x00;
	
	return 0;
}


static uint8_t Expand_Link_Manual_Control(void)
{
	motor->status = upper->host_infor.data;
	
	upper->slave_infor.data_len = 1;
	upper->slave_infor.cmd = 0x03;
	upper->slave_infor.data[0] = 0x01;
	upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
	
	if(ee_sx951w->status == EE_SX951W_UNLOCK)
	{
		if((steering->steering_infor.current_position > 0x0BB8)\
			|| (steering->steering_infor.current_position < 0x39))
		{
			steering->Set_Position(steering, 10);
			HAL_Delay(500);
		}
		
		switch(motor->status)
		{
			case 0x00:
				motor->SetStatus(motor, MOTOR_STOP);
				break;
			case 0x01:
				motor->SetStatus(motor, MOTOR_EXTEND);
				break;
			case 0x02:
				motor->SetStatus(motor, MOTOR_PULL_BACK);
				break;
		}
	}
	else if(ee_sx951w->status == EE_SX951W_LOCK)
	{
		steering->Set_Position(steering, 10);
		HAL_Delay(500);
		
		switch(motor->status)
		{
			case 0x00:
				motor->SetStatus(motor, MOTOR_STOP);
				break;
			case 0x01:
				motor->SetStatus(motor, MOTOR_EXTEND);
				break;
			case 0x02:
				motor->SetStatus(motor, MOTOR_PULL_BACK);
				break;
		}
	}

	upper->host_infor.cmd = 0x00;
	
	return 0;
}


static uint8_t SMSCL_Manual_Control(void)
{
	uint8_t temp = upper->host_infor.data;
	
	switch(temp)
	{
		case 0x01:
			for(uint8_t i = 0; i < 3; i++) steering->Set_Position(steering, 180);
			motor->SetStatus(motor, MOTOR_PULL_BACK);
			break;
		case 0x02:
			for(uint8_t i = 0; i < 3; i++) steering->Set_Position(steering, 0);
			motor->SetStatus(motor, MOTOR_PULL_BACK);
			break;
	}
	
	upper->slave_infor.data_len = 1;
	upper->slave_infor.cmd = 0x04;
	upper->slave_infor.data[0] = 0x01;
	upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
	upper->host_infor.cmd = 0x00;
	
	return 0;
}

static uint8_t Steering_Calibration(void)
{
	upper->slave_infor.data_len = 1;
	upper->slave_infor.cmd = 0x05;
	upper->slave_infor.data[0] = 0x01;
	upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
	upper->host_infor.cmd = 0x00;
	
	return 0;
}


static uint8_t Partial_Discharge_ReadOnly(void)
{
	/* Turn on the partial discharge switch. */
	if(ee_sx951w->status == EE_SX951W_UNLOCK)
	{
		upper->slave_infor.data_len = 1;
		upper->slave_infor.cmd = 0x06;
		upper->slave_infor.data[0] = 0x03;
		
		upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
	}
	else if(ee_sx951w->status == EE_SX951W_LOCK)
	{
		upper->slave_infor.data_len = 1;
		upper->slave_infor.cmd = 0x06;
		upper->slave_infor.data[0] = 0x01;
		
		upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
	}
//	TEV->Set_Switch(TEV, TEV_ON);
//	
//	HAL_Delay(5000);
//	
//	TEV->Send_Instruction_And_Get_Data(TEV, TEV_MODE);
//	
//	TEV->Set_Switch(TEV, TEV_OFF);
	
//	upper->slave_infor.data_len = 2;
//	upper->slave_infor.cmd = 0x06;
//	upper->slave_infor.data[0] = (TEV->TEV_info.tev_data[2] >> 8) & 0xFF;
//	upper->slave_infor.data[1] = TEV->TEV_info.tev_data[2] & 0xFF;
//	
//	upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
	
	upper->host_infor.cmd = 0x00;
	
	return 0;
}


static uint8_t Test_Procedure(void)
{
		upper->slave_infor.data_len = 1;
		upper->slave_infor.cmd = 0x07;
		upper->slave_infor.data[0] = 0x03;
		
		upper->Return_Status(upper, upper->slave_infor.data, upper->slave_infor.data_len);
		
		upper->host_infor.cmd = 0x00;
		return 0;
}







































