  /******************************************************************************
 *
 * Module:KEYPAD - Driver
 *
 * File Name: keypad.c
 *
 * Description: General driver used to operate 4x3 and 4x4 keypads
 *
 * Author: Kassem
 *
 *******************************************************************************/

#include "keypad.h"


#if (N_col == 3) /*#if to select the functions and code of the keypad 4x3*/
static uint8 KeyPad_4x3_adjustKeyNumber(uint8 button_number);
/*function responsible for adjusting the output of some buttons in 4x3 keypad*/
#elif (N_col == 4) /*#if to select the functions and code of the keypad 4x4*/
static uint8 KeyPad_4x4_adjustKeyNumber(uint8 button_number);
/*function responsible for adjusting the output of some buttons in 4x4 keypad*/
#endif


uint8 KeyPad_getPressedKey(void){
	uint8 col,row;
	while(1)
	{
		for(col=0;col<N_col;col++) /*colomns loop*/
		{
			_delay_ms(250); /*delay to make the keypad wait after accepting an input from the user*/
			/*each loop a different column is output and the others are input*/
			KEYPAD_PORT_DIR = (0b00010000<<col);
			/*enable internal pull up resistor for rows and clear the output pin of column*/
			KEYPAD_PORT_OUT = (~(0b00010000<<col));
			for(row=0;row<N_row;row++) /*rows loop*/
			{
				if(BIT_IS_CLEAR(KEYPAD_PORT_IN,row))
				{
					/*
					 * if a switch is pressed in the current row apply the equation to-
					 * know the value of the pressed key
					 */
					#if (N_col == 3)
						return KeyPad_4x3_adjustKeyNumber((row*N_col)+col+1); /*return the value of the pressed key in keypad 4x3*/
					#elif (N_col == 4)
						return KeyPad_4x4_adjustKeyNumber((row*N_col)+col+1); /*return the value of the pressed key in keypad 4x4*/
					#endif
				}

			}
		}
	}
}

#if (N_col == 3)

static uint8 KeyPad_4x3_adjustKeyNumber(uint8 button_number)
{
	switch(button_number)
	{
	/*
	 * function responsible for adjusting the output of some buttons in 4x3 keypad
	 */
		case 10: return '*';
				 break;
		case 11: return 0;
				 break;
		case 12: return '#';
				 break;
		default: return button_number;
	}
}

#elif (N_col == 4)

static uint8 KeyPad_4x4_adjustKeyNumber(uint8 button_number)
{
	/*
	 * function responsible for adjusting the output of some buttons in 4x4 keypad
	 */
	switch(button_number)
	{
		case 1: return 7;
				break;
		case 2: return 8;
				break;
		case 3: return 9;
				break;
		case 4: return '%';
				break;
		case 5: return 4;
				break;
		case 6: return 5;
				break;
		case 7: return 6;
				break;
		case 8: return '*';
				break;
		case 9: return 1;
				break;
		case 10: return 2;
				break;
		case 11: return 3;
				break;
		case 12: return '-';
				break;
		case 13: return 13;
				break;
		case 14: return 0;
				break;
		case 15: return '=';
				break;
		case 16: return '+';
				break;
		default: return button_number;
	}
}

#endif
