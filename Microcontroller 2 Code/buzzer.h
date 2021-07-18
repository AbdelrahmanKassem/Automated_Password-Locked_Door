/*
 * buzzer.h
 *
 *  Created on: Dec 16, 2020
 *      Author: arahm
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define BUZZER_DDR DDRB
#define BUZZER_PORT PORTB
#define BUZZER PB7

void BUZZER_init(void);
void BUZZER_on(void);
void BUZZER_off(void);

#endif /* BUZZER_H_ */
