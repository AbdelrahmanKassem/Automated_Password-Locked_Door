/*
 * motor.h
 *
 *  Created on: Dec 16, 2020
 *      Author: arahm
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define INPUT1_DDR DDRB
#define INPUT2_DDR DDRB
#define ENABLE_DDR DDRB
#define INPUT1_PORT PORTB
#define INPUT2_PORT PORTB
#define ENABLE_PORT PORTB
#define INPUT1 PB0
#define INPUT2 PB1
#define ENABLE PB2

void MOTOR_init(void);
void MOTOR_runCW(void);
void MOTOR_runACW(void);
void MOTOR_reverse(void);
void MOTOR_stop(void);

#endif /* MOTOR_H_ */
