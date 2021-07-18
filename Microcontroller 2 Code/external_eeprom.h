  /****************************************************************************************
 *
 * Module: EXTERNAL EEPROM - Configuration
 *
 * File Name: external_eeprom.h
 *
 * Description: external_eeprom.h which holds all function prototypes and definitions of the
 * 				external_eeprom.c and also important libraries used by external_eeprom.c
 *
 * Author: Kassem
 *
 ******************************************************************************************/

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

/******************************Libraries********************************/

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"
#include "i2c.h"


/*******************************Definitions******************************/

#define ERROR 0
#define SUCCESS 1


/**************************Function Prototypes***************************/

void EEPROM_init(TwiConfig *twiInit_Ptr) ;
uint8 EEPROM_writeByte(uint16 a_addr, uint32 a_data, uint8 a_numOfBytes);
uint8 EEPROM_readByte(uint16 a_addr,uint32 *data_Ptr , uint8 a_numOfBytes);

#endif /* EXTERNAL_EEPROM_H_ */
