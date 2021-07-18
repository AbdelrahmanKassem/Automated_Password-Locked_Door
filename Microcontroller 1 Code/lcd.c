  /******************************************************************************
 *
 * Module: LCD - Driver
 *
 * File Name: lcd.c
 *
 * Description: driver used to operate different sizes of lcd screens
 *
 * Author: Kassem
 *
 *******************************************************************************/

#include "lcd.h"



void LCD_init(void)
{
	/*
	 * Configure the control pins(E,RS,RW) as output pins
	 */
	LCD_CTRL_DDR |= (1<<RS) | (1<<RW) | (1<<E);
	CLEAR_BIT(LCD_CTRL_PORT,RS);
	CLEAR_BIT(LCD_CTRL_PORT,RW);
	SET_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1); /* add delay for lcd processing */
	#if(LCD_LINES == 1 && LCD_MODE == 8)
	LCD_DATA_DDR=0XFF;/* Configure the data port as output port */
	LCD_sendCommand(ONE_LINE_EIGHT_BIT_MODE);
	#elif(LCD_LINES == 1 && LCD_MODE == 4)
	LCD_DATA_DDR|= LCD_SEL_PINS;
	LCD_sendCommand(ONE_LINE_FOUR_BIT_MODE);
	#elif(LCD_LINES == 2 && LCD_MODE == 8)
	LCD_DATA_DDR=0XFF;/* Configure the data port as output port */
	LCD_sendCommand(TWO_LINE_EIGHT_BIT_MODE);
	#elif(LCD_LINES == 2 && LCD_MODE == 4)
	LCD_DATA_DDR|= LCD_SEL_PINS;
	LCD_sendCommand(TWO_LINE_FOUR_BIT_MODE);
	#endif
	LCD_sendCommand(CURSOR_OFF);/* turn off cursor */
	LCD_sendCommand(CLEAR_DISPLAY_SCREEN); /* clear LCD at the beginning */

}
void LCD_sendCommand(uint8 command)
{
	CLEAR_BIT(LCD_CTRL_PORT,RS);/* Instruction  RS=0 */
	_delay_ms(1); /* add delay for lcd processing */
	CLEAR_BIT(LCD_CTRL_PORT,RW);/* Instruction  RW=0 to write data */
	_delay_ms(1); /* add delay for lcd processing */
	SET_BIT(LCD_CTRL_PORT,E);/* Enable LCD */
	_delay_ms(1);/* add delay for lcd processing */
	#if (LCD_MODE == 8)
	LCD_DATA_PORT = command;
	#elif (LCD_MODE == 4)
	LCD_DATA_PORT=(LCD_DATA_PORT & LCD_SEL_PINS ) | (command & LCD_SEL_PINS);
	_delay_ms(1);
	command = (command << 4) | (command >> 4);
	LCD_DATA_PORT=(LCD_DATA_PORT & LCD_SEL_PINS ) | (command & LCD_SEL_PINS);
	#endif
	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,E);/* Clear E in lcd */
	_delay_ms(1);
}
void LCD_displayCharacter(uint8 data)
{
	SET_BIT(LCD_CTRL_PORT,RS); /* Instruction  RS=1 */
	_delay_ms(1); /* add delay for lcd processing */
	CLEAR_BIT(LCD_CTRL_PORT,RW);
	_delay_ms(1); /* add delay for lcd processing */
	SET_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1); /* add delay for lcd processing */
	#if (LCD_MODE == 8)
	LCD_DATA_PORT = data;
	#elif (LCD_MODE == 4)
	LCD_DATA_PORT=(LCD_DATA_PORT & LCD_SEL_PINS ) | (data & LCD_SEL_PINS);
	_delay_ms(1);
	data = (data << 4) | (data >> 4);
	LCD_DATA_PORT=(LCD_DATA_PORT & LCD_SEL_PINS ) | (data & LCD_SEL_PINS);
	#endif
	_delay_ms(1); /* add delay for lcd processing */
	CLEAR_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1); /* add delay for lcd processing */
}
void LCD_displayString(uint8* string)
{
	uint8 i=0;
	while(string[i] != '\0') /*loop on the string till reach \0 */
	{
		LCD_displayCharacter(string[i]);
		i++;
		#if (LCD_LINES == 2)
		if (i == 16) /*jump to next line in 2 line displays*/
		{
			LCD_sendCommand(CURSOR_BEGIN_LINE_2);
		}
		#endif
	}
}
void LCD_goToRowColumn(uint8 row , uint8 col)
{
	uint8 i=1;
	if (row == 0) /*jump to first line if requested row is 0 */
	{
		LCD_sendCommand(CURSOR_BEGIN_LINE_1);
	}
	else if ( row == 1) /*jump to second line if requested row is 0 */
	{
		LCD_sendCommand(CURSOR_BEGIN_LINE_2);
	}
	while (i < col) /*Shift cursor to requested column by looping on cursor right command*/
	{
		LCD_sendCommand(SHIFT_CURSOR_RIGHT);
		i++;
	}
}
void LCD_displayStringRowColumn(uint8* string , uint8 row , uint8 col)
{
	LCD_goToRowColumn(row , col); /* go to to the required LCD position */
	LCD_displayString(string);/* display the string */

}
void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_DISPLAY_SCREEN);/*clear display */
}
void LCD_integerToString(int num)
{
	uint8 string [16];/* String to hold the ascii result */
	itoa(num, string, 10);/*convert from integer to asci and save asci in the string*/
	LCD_displayString(string);/*display string*/
}

