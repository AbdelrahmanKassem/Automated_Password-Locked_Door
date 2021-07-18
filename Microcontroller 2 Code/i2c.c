  /******************************************************************************
 *
 * Module:I2C - Driver
 *
 * File Name: i2c.c
 *
 * Description: General driver used to operate i2c in AVR micro-controller
 *
 * Author: Kassem
 *
 *******************************************************************************/

#include "i2c.h"

void TWI_init(const TwiConfig *twiInit_Ptr)  /*function receives the configuration structure by reference to initialize the i2c */
{
	uint8 prescaler=1; uint32 twbr=0; uint8 twps=0;   /*Calculating TWBR and TWSR value from clock frequency entered by user*/
	while(prescaler<=64)
	{
		twbr = (((F_CPU/(twiInit_Ptr->s_clockFreq))-16)/(2*(prescaler))); /*calculating twbr using given SCL equation*/
		if(twbr<256)
		{
			break; /*if the value of twbr fits in an 8 bit register then we finished our calculations*/
		}
		else
		{
			/*
			 * if the value of twbr doesn't fit in 8 bit register
			 * multiply prescaler by 4
			 * add one to twps in an attempt to decrease the value of twbr in the next try
			 */
			prescaler=prescaler*4;
			twps = twps+1;
		}
	}
	TWBR = (uint8)twbr; /*saving calculated value of TWBR to the register*/
	TWSR = (TWSR & 0xFC) | ( twps & 0x03); /*saving the TWSR least significant two bits to configure the i2c clock*/
	TWAR = ((twiInit_Ptr->s_slaveAddress)<<1);  /*disable general call recognition and adding slave address to the 7 MSBs*/
	TWCR = (1<<TWEN); /*TWI enable Bit*/

}
void TWI_start(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA); /*CLEAR FLAG + ENABLE TWI + SEND START BIT*/
	while(BIT_IS_CLEAR(TWCR,TWINT)) /*wait until start is sent*/
	{

	}
}
void TWI_stop(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); /*CLEAR FLAG + ENABLE TWI + SEND STOP BIT*/
}
void TWI_write(uint8 a_data) /*function that takes the data the user want to send*/
{
	TWDR = a_data; /*saving data in the register to be transmited*/
	TWCR = (1<<TWINT) | (1<<TWEN) ;/*CLEAR FLAG + ENABLE TWI*/
	while(BIT_IS_CLEAR(TWCR,TWINT))/*wait for the bit to be sent*/
	{

	}
}
uint8 TWI_readWithACK(void)  /*function returns data that needs to be received*/
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); /*CLEAR FLAG + ENABLE TWI + SEND ACK AFTER DATA TRANSMITION*/
	while(BIT_IS_CLEAR(TWCR,TWINT)) /*wait till data is received*/
	{

	}
	return TWDR; /*return data*/
}
uint8 TWI_readWithNACK(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);/*CLEAR FLAG + ENABLE TWI + sending ack off*/
	while(BIT_IS_CLEAR(TWCR,TWINT)) /*wait till data is received*/
	{

	}
	return TWDR; /*return data*/
}
uint8 TWI_getStatus(void)
{
	/*
	 * get the last 5 bits of the status register and masking the first 3 bits
	 */
	uint8 status = TWSR & 0xF8;
	return status; /*return status*/
}
