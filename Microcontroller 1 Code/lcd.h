  /******************************************************************************
 *
 * Module: LCD - Configuration
 *
 * File Name: lcd.h
 *
 * Description: lcd.h which holds all function prototypes and definitions of the lcd.c
 * 				and also important libraries used by lcd.c
 *
 * Author: Kassem
 *
 *******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

/******************************Libraries********************************/

#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"

/******************************Definitions********************************/

#define LCD_MODE 8
#define LCD_LINES 2

#if (LCD_MODE == 4)
#define LCD_SEL_PINS 0xF0   /*UPPER PART*/
#endif

#define LCD_DATA_DDR DDRA
#define LCD_DATA_PORT PORTA
#define LCD_CTRL_DDR DDRC
#define LCD_CTRL_PORT PORTC
#define RS PC0
#define RW PC1
#define E  PC2

/******************************COMMANDS********************************/

#define CURSOR_BEGIN_LINE_1 0X80
#define CURSOR_BEGIN_LINE_2 0XC0
#define SHIFT_CURSOR_RIGHT 0X14
#define CLEAR_DISPLAY_SCREEN 0X01
#define ONE_LINE_EIGHT_BIT_MODE 0X30
#define ONE_LINE_FOUR_BIT_MODE 0X20
#define TWO_LINE_EIGHT_BIT_MODE 0X38
#define TWO_LINE_FOUR_BIT_MODE 0X28
#define CURSOR_OFF 0x0C

/******************************Function Prototypes********************************/

void LCD_init(void);
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_displayString(uint8* string);
void LCD_goToRowColumn(uint8 row , uint8 col);
void LCD_displayStringRowColumn(uint8* string , uint8 row , uint8 col);
void LCD_clearScreen(void);
void LCD_integerToString(int num);

#endif /* LCD_H_ */
