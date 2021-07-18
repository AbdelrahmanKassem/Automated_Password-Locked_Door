  /******************************************************************************
 *
 * Title:Application of Micro-controller 2
 *
 * File Name: mc2_application.c
 *
 * Description: Application using Atmega16 to control a door using password with
 * 				the ability to change the password any time where MC2 is responsible
 * 				for the eeprom control, Door Control & Buzzer.
 *
 * Author: Kassem
 *
 *******************************************************************************/


#include "mc2_application.h"

uint32 Password=0; uint32 overflow=0;
Timer_ConfigType TimerSettings = {TIMER_1,COMPARE_MATCH,F_CPU_1024,0,0,0}; /*initializing timer configuration struct*/

uint8 Check_Flag(void)
{
	/*
	 * function responsible of cheching the flag status in the EEPROM and returning its status
	 * the flag status determines if a password has been set before or not
	 */
	uint32 flag_status;
	EEPROM_readByte(FLAG_ADDRESS,&flag_status , 1);
	if(flag_status == SUCCESS)
	{
		/*
		 * return SUCCESS flag if the was set
		 */
		return SUCCESS;
	}
	else
	{
		/*
		 * return FAIL flag if the flag wasn't set
		 */
		return FAIL;
	}
}

void Reply(uint8 state) //state argument should be SUCCESS OR FAIL
{
	/*
	 * send a FLAG to MC1 to determine current state or to help with synchronization
	 */
	UART_sendByte(state);
}

void Read_Stored_Password(void)
{
	/*
	 * read password from address defined in PASS_ADDRESS in EEPROM
	 * read the 4 byte password stored in the EEPROM
	 * store then in Password variable
	 */
	EEPROM_readByte(PASS_ADDRESS,&Password , 4);
}

void Send_Password(void)
{
	/*
	 * send stored password to MC1 so password validation can occur without the need of extra-
	 * communication between the two micro-controllers and the eeprom
	 */
	uint8 i=0;
	for(i=0;i<4;i++)
	{
		/*
		 * loop to send the 4 byte password
		 * sending the first byte in the LSBs
		 * then shifting right by 8 bits each loop to send the next byte
		 */
		UART_sendByte((uint8)(Password>>(i*8)));
	}

}

void Receive_Password() /*receive password from MC1*/
{
	uint8 i; Password=0;
	for(i=0;i<4;i++)
	{
		/*
		 * loop to receive the 4 byte password
		 * receiving the first byte in the LSBs
		 * then receiving the next byte in the next 8 bits of the variable each loop
		 */
		Password |= (((uint32)UART_recieveByte())<<(i*8));
	}
}
void Save_Password(void)
{
	/*
	 * update the flag status
	 * save received password in EEPROM
	 */
	EEPROM_writeByte(FLAG_ADDRESS,(uint32)SUCCESS , 1);
	EEPROM_writeByte(PASS_ADDRESS,Password , 4);
}

uint8 Receive_Command(void)
{
	/*
	 * receive command from MC1 to decide the next action
	 */
	return (UART_recieveByte());
}

void Door_Sequence(void)
{
	TimerSettings.s_compareValue = 23439; //3 seconds
	Timer_init(&TimerSettings); /* initialize timer after updating the compare value in the configuration struct*/
	MOTOR_runCW(); /* starting motor in clock wise direction*/
	while (overflow < 5)  //3x5=15 seconds
	{
		/*wait 15 seconds*/
	}
	Timer_clearTimerValue(TIMER_1); /*starting timer from zero again*/
	overflow=0; /* reset overflow flag*/
	MOTOR_stop(); /*stop the motor*/
	while(overflow < 1) //wait 3 seconds
	{
		/*wait 3 seconds*/
	}
	Reply(SUCCESS); /* send success flag to MC1 for synchronization and to notify that the door will close*/
	Timer_clearTimerValue(TIMER_1); /*starting timer from zero again*/
	overflow=0; /* reset overflow flag*/
	MOTOR_runACW(); /* starting motor in Anti-clock wise direction*/
	while (overflow < 5)  //3x5=15 seconds
	{
		/*wait 15 seconds*/
	}
	Reply(SUCCESS); /* send success flag to MC1 for synchronization and to notify that the door process finished*/
	MOTOR_stop(); /*stop the motor*/
	Timer_deInit(TIMER_1); /* stop the timer*/
	overflow=0; /* clear the overflow flag*/
}

void Alert(void)
{
	/*
	 * responsible for activating the buzzer for 60 seconds
	 */
	TimerSettings.s_compareValue = 46875; //6 seconds
	Timer_init(&TimerSettings); /* initialize timer after updating the compare value in the configuration struct*/
	BUZZER_on(); /* activate buzzer */
	while(overflow < 10) //6x10=60 seconds
	{
		/*wait 60 seconds*/
	}
	Reply(SUCCESS); /* send success flag to MC1 for synchronization and to notify that the Alert process finished*/
	BUZZER_off(); /*turn off the buzzer*/
	Timer_deInit(TIMER_1); /* stop the timer*/
	overflow=0; /* clear the overflow flag*/
}

void Timer_Callback(void)
{
	/*the call back function of the timer*/
	overflow++; /*increase the overflow flag*/
}

int main (void)
{
	UartConfig UartSettings = {NO_PARITY,9600,SINGLE,EIGHT_BITS}; /*setting up the UART configuration structure*/
	TwiConfig I2Csettings = {400000,0}; /*setting up the I2C configuration structure*/
	SREG = (1<<7); /*enable global interrupt*/
	Timer_setCallBack(TIMER_1, Timer_Callback); /*setting the call back function of timer 1*/
	UART_init(&UartSettings); /*initializing the uart and passing the config struct. to it*/
	EEPROM_init(&I2Csettings) ; /*initializing the EEPROM and passing the I2C config struct. to it*/
	MOTOR_init(); /*initializing the motor*/
	BUZZER_init(); /*initializing the buzzer*/
	switch (Check_Flag())
	{
	/*
	 * checking if there is a stored password in the EEPROM or not by checking the flag status
	 */
	case FAIL: /*if there is no password (first time for the user to use the application) */
		Reply(FAIL); /*send FAIL flag to MC1 to make it decide the next step*/
		Receive_Password(); /*receiving the new password from MC1*/
		Save_Password(); /*saving the password to the EEPROM and updating flag status*/
		Reply(SUCCESS); /*send SUCCESS flag to MC1 for synchronization*/
		break;
	case SUCCESS: /*if there is a stored password*/
		Reply(SUCCESS); /*send SUCCESS flag to MC1 to make it decide the next step*/
		Read_Stored_Password(); /*read the stored password from the EEPROM*/
		_delay_ms(100); /*delay for synchronization*/
		Send_Password(); /*sending password to MC1 to perform validation processes locally*/
		break;
	}

	while(1)
	{
		switch(Receive_Command()) /*wait for command from MC1 to serve*/
		{
		case CREATE_PASSWORD: /*received command is CREATE_PASSWORD*/
			Receive_Password(); /*receive new password from MC1*/
			Save_Password(); /*save the new password to the eeprom*/
			Reply(SUCCESS); /*send SUCCESS flag to MC1 to make it decide the next step and for synchronization*/
			break;
		case OPEN_DOOR: /*received command is OPEN_DOOR*/
			Door_Sequence(); /* do the Door sequence */
			break;
		case ALERT: /*received command is ALERT*/
			Alert(); /* do the Alert sequence*/
			break;

		}
	}

}

/* mafata7te4 7aga mn ba3d el midterm 34an el project ya tarek*/
/* el gpa byro7 meni ya tareek XD */
