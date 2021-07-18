 /****************************************************************************************
 *
 * Module: Microcontroller 1 - Configuration
 *
 * File Name: mc1_application.h
 *
 * Description: mc1_application.h which holds all function prototypes & definitions
 * 				of the mc1_application.c and also important libraries used by mc1_application.c
 *
 * Author: Kassem
 *
 ******************************************************************************************/

#ifndef MC1_APPLICATION_H_
#define MC1_APPLICATION_H_

/******************************Libraries********************************/

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad.h"
#include "lcd.h"
#include "uart.h"


/******************************Definitions******************************/

#define F_CPU 8000000UL

#define FAIL 0
#define SUCCESS 1
#define OPEN_DOOR 2
#define ALERT 3
#define CREATE_PASSWORD 4

#endif /* MC1_APPLICATION_H_ */
