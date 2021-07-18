  /****************************************************************************************
 *
 * Module: TIMERS - Configuration
 *
 * File Name: timers.h
 *
 * Description: timers.h which holds all function prototypes, config. structures &
 * 				definitions of the timers.c and also important libraries used by timers.c
 *
 * Author: Kassem
 *
 ******************************************************************************************/

#ifndef TIMERS_H_
#define TIMERS_H_


/******************************Libraries********************************/

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/********************Configuration Struct and enums*********************/

typedef enum{
	TIMER_0, TIMER_1, TIMER_2
}Timer_Number;

typedef enum{
	OVERFLOW, COMPARE_MATCH
}Timer_Mode;

typedef enum{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Prescaler;


typedef struct{
	Timer_Number s_timer;
	Timer_Mode s_mode;
	Timer_Prescaler s_prescaler;
	uint16 s_initValue;
	uint16 s_compareValue;    //also used as compare value A for timer 1
	uint16 s_compareValue_B;

}Timer_ConfigType;


/**************************Function Prototypes************************/

void Timer_init(const Timer_ConfigType *a_TimerConfig_Ptr);
void Timer_setCallBack(Timer_Number a_Timer, void(*a_Ptr)(void));
void Timer_1_CompB_setCallBack(void(*a_Ptr)(void));
uint16 Timer_getTimerValue(Timer_Number a_Timer);
void Timer_clearTimerValue(Timer_Number a_Timer);
void Timer_deInit(Timer_Number a_Timer);

#endif /* TIMERS_H_ */
