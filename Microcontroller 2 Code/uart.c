  /******************************************************************************
 *
 * Module:UART - Driver
 *
 * File Name: uart.c
 *
 * Description: General driver used to operate uart in AVR micro-controller
 *
 * Author: Kassem
 *
 *******************************************************************************/

#include "uart.h"

void UART_init(const UartConfig *uartInit_Ptr)
{
	UCSRA = (1<<U2X); /*double transmission speed */
	/*
	 * Disable Interrupts
	 * Enable Receiver
	 * EnableTransmitter
	 */
	UCSRB = (1<<RXEN) | (1<<TXEN);
	/*
	 * Asynchronous Operation
	 * Disabling parity bit
	 * Using one stop bit
	 */
	UCSRC = (1<<URSEL) ;
	switch (uartInit_Ptr->s_parity)
	{
	/*
	 * configuring parity type according to the number of bits the user requested
	 */
		case EVEN:/*even parity*/
			SET_BIT(UCSRC,UPM1);
			break;
		case ODD:/*odd parity*/
			SET_BIT(UCSRC,UPM1);
			SET_BIT(UCSRC,UPM0);
			break;
		case NO_PARITY:/*no parity*/
			break;
	}

	if (uartInit_Ptr->s_stopBit)
	{
		/*
		 * configuring number of stop bits according to the number the user requested
		 */
			SET_BIT(UCSRC,USBS);
	}
	switch (uartInit_Ptr->s_bitDataNum)
	{
	/*
	 * configuring uart number of data bits according to the number of bits the user requested
	 */
		case FIVE_BITS:/*5 bit data transmition*/
			break;
		case SIX_BITS:/*6 bit data transmition*/
			SET_BIT(UCSRC,UCSZ0);
			break;
		case SEVEN_BITS:/*7 bit data transmition*/
			SET_BIT(UCSRC,UCSZ1);
			break;
		case EIGHT_BITS:/*8 bit data transmition*/
			SET_BIT(UCSRC,UCSZ1);
			SET_BIT(UCSRC,UCSZ0);
			break;
		case NINE_BITS:/*9 bit data transmition*/
			SET_BIT(UCSRB,UCSZ2);
			SET_BIT(UCSRC,UCSZ1);
			SET_BIT(UCSRC,UCSZ0);
			break;
	}

	/*Baud Rate*/
	uint16 baudScale = (F_CPU/(8*(uartInit_Ptr->s_baudRate)))-1 ;/*calculating baud scale from baud rate*/
	/*configuring uart prescaler*/
	UBRRL = (baudScale & 0x00FF);
	UBRRH = (baudScale >> 8);
}

void UART_sendByte(const uint8 a_data)
{
	while(BIT_IS_CLEAR(UCSRA,UDRE)) /*waiting for previous data to be sent*/
	{

	}
	UDR = a_data;/*sending data to register*/
}

uint8 UART_recieveByte(void)
{
	while(BIT_IS_CLEAR(UCSRA,RXC))/*checking that the register completed receiving the data*/
	{

	}
	return UDR;/*saving data from register*/
}

void UART_sendString(const uint8 *str_Ptr)
{
	uint8 i=0;
	while ((str_Ptr[i]) != '\0') /*sending all string characters till \0 */
	{
		UART_sendByte(str_Ptr[i]);
		i++;
	}
}

void UART_receiveString(uint8 *str_Ptr)
{
	uint8 i=0;
	while ((str_Ptr[i]) != '#') /*receiving all string characters till # */
	{
		str_Ptr[i]=UART_recieveByte();
		i++;
	}
	str_Ptr[i]='\0';
}
