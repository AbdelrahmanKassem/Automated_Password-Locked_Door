 /****************************************************************************************
 *
 * Module: Microcontroller 2 - Configuration
 *
 * File Name: mc2_application.h
 *
 * Description: mc2_application.h which holds all function prototypes & definitions
 * 				of the mc2_application.c and also important libraries used by mc2_application.c
 *
 * Author: Kassem
 *
 ******************************************************************************************/

#ifndef MC2_APPLICATION_H_
#define MC2_APPLICATION_H_


/******************************Libraries********************************/

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c.h"
#include "timers.h"
#include "external_eeprom.h"
#include "uart.h"
#include "motor.h"
#include "buzzer.h"


/******************************Definitions******************************/

#define F_CPU 8000000UL

#define FAIL 0
#define SUCCESS 1
#define OPEN_DOOR 2
#define ALERT 3
#define CREATE_PASSWORD 4
#define FLAG_ADDRESS 0x01
#define PASS_ADDRESS 0x05

#endif /* MC2_APPLICATION_H_ */
