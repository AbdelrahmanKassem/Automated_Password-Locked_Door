  /******************************************************************************
 *
 * Module:EXTERNAL EEPROM - Driver
 *
 * File Name: i2c.c
 *
 * Description: General driver used to operate i2c in AVR micro-controller
 *
 * Author: Kassem
 *
 *******************************************************************************/

#include "external_eeprom.h"


void EEPROM_init(TwiConfig *twiInit_Ptr)
{
	/*
	 * receiving the I2C configuration struct by reference
	 * initializing the I2C
	 */
	TWI_init(twiInit_Ptr);
}
uint8 EEPROM_writeByte(uint16 a_addr, uint32 a_data, uint8 a_numOfBytes)
{
	uint8 i;
	for (i=0;i<a_numOfBytes;i++) /*writing a number of times according to the number of bits added by the user*/
	{
		TWI_start(); /*sending start bit*/
		if (TWI_getStatus() != TW_START)
		{
			/*
			 * checking if the status register matches the current step
			 * returning error and terminating the process if not
			 */
		 return ERROR;
		}

		/*
		 * sending the 3 bits slave address next to 1010 in the MSBs
		 * sending write request in the LSB
		 */
		TWI_write((uint8)(0xA0 | ((a_addr & 0x0700)>>7)));
		if (TWI_getStatus() != TW_MT_SLA_W_ACK)
		{
			/*
			 * checking if the status register matches the current step
			 * returning error and terminating the process if not
			 */
		 return ERROR;
		}

		/*
		 * sending the word address of the data we want to write to the eeprom
		 * increasing the address by 1 when we repeat the process to write more than one byte
		 */
		TWI_write((uint8)(a_addr+i));
		if (TWI_getStatus() != TW_MT_DATA_ACK)
		{
			/*
			 * checking if the status register matches the current step
			 * returning error and terminating the process if not
			 */
		 return ERROR;
		}

		/*
		 * saving the first 8 bits of the data sent by the user to the DataToWrite variable that the data if it will be transmitted
		 * shifting the data sent by the user 8 bits each time when we repeat the process to send then next byte
		 */
		uint8 dataToWrite = (a_data>>(i*8)) ;
		TWI_write(dataToWrite); /*Transmiting the data in the variable (DataToWrite)*/
		if (TWI_getStatus() != TW_MT_DATA_ACK)
		{
			/*
			 * checking if the status register matches the current step
			 * returning error and terminating the process if not
			 */
		 return ERROR;
		}
		TWI_stop(); /*sending stop bit*/
		_delay_ms(10);

	}
	return SUCCESS;
}

uint8 EEPROM_readByte(uint16 a_addr,uint32 *data_Ptr , uint8 a_numOfBytes)
{
	TWI_start(); /*sending start bit*/
	if (TWI_getStatus() != TW_START)
	{
		/*
		 * checking if the status register matches the current step
		 * returning error and terminating the process if not
		 */
	 return ERROR;
	}

	/*
	 * sending the 3 bits slave address next to 1010 in the MSBs
	 * sending write request in the LSB
	 */
	TWI_write((uint8)(0xA0 | ((a_addr & 0x0700)>>7)));
	if (TWI_getStatus() != TW_MT_SLA_W_ACK)
	{
		/*
		 * checking if the status register matches the current step
		 * returning error and terminating the process if not
		 */
	 return ERROR;
	}

	/*sending the word address of the data we want to write to the eeprom*/
	TWI_write((uint8)(a_addr));
	if (TWI_getStatus() != TW_MT_DATA_ACK)
	{
		/*
		 * checking if the status register matches the current step
		 * returning error and terminating the process if not
		 */
	 return ERROR;
	}

	TWI_start();/*resending start bit*/
	if (TWI_getStatus() != TW_REP_START)
	{
		/*
		 * checking if the status register matches the current step
		 * returning error and terminating the process if not
		 */
	 return ERROR;
	}

	/*
	 * resending the 3 bits slave address next to 1010 in the MSBs
	 * sending read request in the LSB
	 */
	TWI_write((uint8)(0xA0 | ((a_addr & 0x0700)>>7))| 0x01);
	if (TWI_getStatus() != TW_MT_SLA_R_ACK)
	{
		/*
		 * checking if the status register matches the current step
		 * returning error and terminating the process if not
		 */
	 return ERROR;
	}

	uint8 counter; uint32 readData; *data_Ptr=0;

	/*reading from eeprom according to number of bits requested by the user*/
	for(counter=0;counter<(a_numOfBytes);counter++)
	{
		readData=0;
		if (counter<(a_numOfBytes-1)) /*reading with ack all the time except last time*/
		{
			readData = TWI_readWithACK(); /*saving the latest received byte in the (readData) variable*/

			if (TWI_getStatus() != TW_MR_DATA_ACK)
			{
				/*
				 * checking if the status register matches the current step
				 * returning error and terminating the process if not
				 */
			 return ERROR;
			}
		}
		else
		{
			readData=TWI_readWithNACK(); /*saving the latest received byte in the (readData) variable*/
			if (TWI_getStatus() != TW_MR_DATA_NACK)
			{
				/*
				 * checking if the status register matches the current step
				 * returning error and terminating the process if not
				 */
			 return ERROR;
			}
		}
		*data_Ptr = *data_Ptr | (readData<<(counter*8)); /*putting first received byte in LSBs and the next byte in the next 8 bits and so on*/
	}

	TWI_stop(); /*sending stop bit*/
	return SUCCESS;
}
