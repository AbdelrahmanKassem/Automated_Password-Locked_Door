/*
 * buzzer.c
 *
 *  Created on: Dec 16, 2020
 *      Author: arahm
 */



#include "buzzer.h"

void BUZZER_init(void)
{
	SET_BIT(BUZZER_DDR,BUZZER);
	CLEAR_BIT(BUZZER_PORT,BUZZER);
}

void BUZZER_on(void)
{
	SET_BIT(BUZZER_PORT,BUZZER);
}

void BUZZER_off(void)
{
	CLEAR_BIT(BUZZER_PORT,BUZZER);
}
