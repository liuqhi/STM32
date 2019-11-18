
#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f1xx_hal.h"

typedef struct __motor_t motor_t;

typedef struct __motor_t
{
	uint8_t status;
	
	uint8_t (*SetStatus)(motor_t*, uint8_t);
	
} motor_t;


typedef enum
{
	MOTOR_STOP = 0,
	MOTOR_EXTEND = 1,
	MOTOR_PULL_BACK = 2
} MOTOR_RunState;



motor_t* Motor_Creat(void);

void Motor_Destroy(motor_t* motor);




#endif























