 /****************************************************************************************
 *
 * Module: KEYPAD - Configuration
 *
 * File Name: keypad.h
 *
 * Description: keypad.h which holds all function prototypes definitions of the keypad.c
 *              and also important libraries used by keypad.c
 *
 * Author: Kassem
 *
 ******************************************************************************************/



#ifndef KEYPAD_H_
#define KEYPAD_H_

/******************************Libraries********************************/

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"


/******************************Definitions******************************/

#define N_col 3
#define N_row 4
#define KEYPAD_PORT_OUT PORTB
#define KEYPAD_PORT_IN  PINB
#define KEYPAD_PORT_DIR DDRB

/**************************Function Prototypes************************/

uint8 KeyPad_getPressedKey(void);

#endif /* KEYPAD_H_ */
