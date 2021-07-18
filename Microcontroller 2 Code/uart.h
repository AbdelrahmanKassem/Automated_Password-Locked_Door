  /****************************************************************************************
 *
 * Module: uart - Configuration
 *
 * File Name: uart.h
 *
 * Description: uart.h which holds all function prototypes, config. structures & definitions
 * 				of the lcd.c and also important libraries used by uart.c
 *
 * Author: Kassem
 *
 ******************************************************************************************/

#ifndef UART_H_
#define UART_H_

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

/******************************Libraries********************************/

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>


/********************Configuration Struct and enums*********************/

typedef enum
{
	NO_PARITY,EVEN,ODD
}ParityType;

typedef enum
{
	SINGLE,DOUBLE
}StopBitNum;

typedef enum
{
	FIVE_BITS,SIX_BITS,SEVEN_BITS,EIGHT_BITS,NINE_BITS
}DataNum;

typedef struct
{
	ParityType s_parity;
	uint16 s_baudRate;
	StopBitNum s_stopBit;
	DataNum s_bitDataNum;
}UartConfig;


/**************************Function Prototypes************************/

void UART_init(const UartConfig *uartInit_Ptr);

void UART_sendByte(const uint8 a_data);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *str_Ptr);

void UART_receiveString(uint8 *str_Ptr);

#endif /* UART_H_ */
