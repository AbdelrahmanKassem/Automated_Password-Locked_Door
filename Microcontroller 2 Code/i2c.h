  /****************************************************************************************
 *
 * Module: I2C - Configuration
 *
 * File Name: i2c.h
 *
 * Description: i2c.h which holds all function prototypes, config. structures & definitions
 * 				of the i2c.c and also important libraries used by i2c.c
 *
 * Author: Kassem
 *
 ******************************************************************************************/

#ifndef I2C_H_
#define I2C_H_

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

/************************ Configuration Struct **************************/

typedef struct
{
	uint32 s_clockFreq;
	uint8 s_slaveAddress;
}TwiConfig;


/**************************Function Prototypes***************************/

void TWI_init(const TwiConfig *twiInit_Ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8 a_data);
uint8 TWI_readWithACK(void); //read with send Ack
uint8 TWI_readWithNACK(void); //read without send Ack
uint8 TWI_getStatus(void);


/*******************************Definitions******************************/

/* I2C Status Bits in the TWSR Register */
#define TW_START         0x08 // start has been sent
#define TW_REP_START     0x10 // repeated start
#define TW_MT_SLA_W_ACK  0x18 // Master transmit ( slave address + Write request ) to slave + Ack received from slave
#define TW_MT_SLA_R_ACK  0x40 // Master transmit ( slave address + Read request ) to slave + Ack received from slave
#define TW_MT_DATA_ACK   0x28 // Master transmit data and ACK has been received from Slave.
#define TW_MR_DATA_ACK   0x50 // Master received data and send ACK to slave
#define TW_MR_DATA_NACK  0x58 // Master received data but doesn't send ACK to slave


#endif /* I2C_H_ */
