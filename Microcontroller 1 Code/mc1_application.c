  /******************************************************************************
 *
 * Title:Application of Micro-controller 1
 *
 * File Name: mc1_application.c
 *
 * Description: Application using Atmega16 to control a door using password with
 * 				the ability to change the password any time where MC1 is reponsible
 * 				for any direct user interaction using LCD and Keypad.
 *
 * Author: Kassem
 *
 *******************************************************************************/

#include "mc1_application.h"

uint32 Password=0 ; /*global variable to hold the password*/
uint32 Stored_Password=-1; /*global variable to store password sent by the MC2*/

uint8 Receive_Flag(void)
{
	/*
	 * the function receives a flag from MC2
	 * this flag determines if it is the first time for the user to start the app.
	 * function returns the flag
	 */
	uint8 flag;
	flag = UART_recieveByte();
	return flag;
}

void Receive_Stored_Password()
{
	/*
	 * the function receives the 4 byte password from MC2
	 * it saves the password in the Stored_Password variable
	 * by this technique we can compare passwords without sending them to MC2 every time-
	 * thus optimizing the code and saving time
	 */
	uint8 i; Stored_Password=0;
	for(i=0;i<4;i++)
	{
		/*
		 * loop to receive all 4 bytes
		 * shifting the received byte 8 bits to the right compared to the latest received byte
		 * by this technique we store the first byte in the LSBs and next bytes are in next to it
		 */
		Stored_Password |= (((uint32)UART_recieveByte())<<(i*8));
	}

}

void Create_Password(void)
{
	LCD_clearScreen(); /*clear screen before starting the process*/
	uint8 PasswordArray1[10] ; /*array to hold the first password entered by the user*/
	uint8 PasswordArray2[10] = {0};/*array to hold the second password entered by the user*/
	uint32 Password2 = -1; uint8 i=0;
	do
	{
		LCD_displayString("Create New Pass:"); /*Displays Create New Pass: on the screen*/
		LCD_goToRowColumn( 1 , 0 ); /*moves cursor to the next line*/
		for(i=0;i<5;i++)
		{
			/*
			 * loop to get the five digits entered from the user on the keypad
			 * adding '0' to the returned value of the keypad to transform the number into asci format
			 * displaying * on the screen after each digit entered by the user
			 */
			PasswordArray1[i] = (KeyPad_getPressedKey() + '0');
			LCD_displayCharacter('*');
		}
		PasswordArray1[5] = '\0'; /*adding null in the end of the array for atoi function to work*/
		/*
		 * converting string into integer for easy compare
		 * having the password as integer makes us able to send it by uart or i2c in only 4 bytes
		 * also it makes any future increase in the number of digits of the password much easier and-
		 * the change in the code is minimum because we will send and receive it in only 4 bytes anyway.
		 */
		Password = atoi(PasswordArray1);

		LCD_clearScreen(); /*clear screen before starting the process again*/

		LCD_displayString("Type Pass Again:"); /*Displays Type Pass Again: on the screen*/
		LCD_goToRowColumn( 1 , 0 ); /*moves cursor to the next line*/
		for(i=0;i<5;i++)
		{
			/*
			 * loop to get the five digits entered from the user on the keypad (second time)
			 * adding '0' to the returned value of the keypad to transform the number into asci format
			 * displaying * on the screen after each digit entered by the user
			 */
			PasswordArray2[i] = (KeyPad_getPressedKey() + '0');
			LCD_displayCharacter('*');
		}
		PasswordArray2[i] = '\0'; /*adding null in the end of the array for atoi function to work*/
		/*
		 * converting string into integer for easy compare
		 * having the password as integer makes us able to send it by uart or i2c in only 4 bytes
		 * also it makes any future increase in the number of digits of the password much easier and-
		 * the change in the code is minimum because we will send and receive it in only 4 bytes anyway.
		 */
		Password2 = atoi(PasswordArray2);
		LCD_clearScreen(); /*clear screen after finishing the process*/
		if (Password != Password2)
		{
			LCD_displayString("Pass Mismatch !"); /*Displays Pass Mismatch ! on the screen*/
			LCD_goToRowColumn( 1 , 0 ); /*moves cursor to the next line*/
			LCD_displayString("Try Again .."); /*Displays Try Again .. on the screen*/
			_delay_ms(1500); /*delay to make the user read the messege*/
			LCD_clearScreen(); /*clear screen after finishing the process*/
		}
	}while (Password != Password2); /* repeat the hole process if the two entered passwords aren't equal*/
	Stored_Password=Password; /*updating the Store_Password to the latest password*/


}

uint8 Save_Password(void)
{
	/*
	 * function sends the password to MC2 to save it in the EEPROM
	 */
	uint8 i=0;uint8 save_status;
	for(i=0;i<4;i++)
	{
		/*
		 * loop to send the 4 bytes of the password
		 * sending the first 8 LSBs then shifting and sending the next 8 bits each loop
		 */
		UART_sendByte((uint8)(Password>>(i*8)));
	}
	save_status = UART_recieveByte(); /*wait for MC2 to reply to keep the synchronization between them*/
	return save_status; /*return save_status to facilitate debug if needed*/
}

void Enter_Password(void)
{
	/*
	 * function responsible for the process of entering the password after being created
	 */
	uint8 PasswordArray1[5] = {0}; /*array to hold the password entered by the user*/
	LCD_clearScreen();/*clear screen in the start of the process*/
	LCD_displayString("Enter Password :"); /*Displays Enter Password : on the screen*/
	LCD_goToRowColumn( 1 , 0 ); /*moves cursor to the next line*/
	uint8 i=0;
	for(i=0;i<5;i++)
	{
		/*
		 * loop to get the five digits entered from the user on the keypad
		 * adding '0' to the returned value of the keypad to transform the number into asci format
		 * displaying * on the screen after each digit entered by the user
		 */
		PasswordArray1[i] = (KeyPad_getPressedKey() + '0');
		LCD_displayCharacter('*');
	}
	Password = atoi(PasswordArray1); /*converting string to integer to compare with stored password in validation phase*/
	LCD_clearScreen();/*clear screen after finishing the process*/
}

uint8 Validate_Password(void)
{
	/*
	 * the function compares the password entered by the user to the stored password
	 * if they are equal it returns a success flag
	 * else it returns a fail flag
	 */
	if(Password == Stored_Password)
	{
		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
}

void Send_OpenDoor_Command(void)
{

	uint8 door_status;
	LCD_clearScreen(); /*clear screen in the start of the process*/
	UART_sendByte(OPEN_DOOR); /*send open door command to MC2 to start the door sequence*/
	LCD_displayString("Opening Door"); /*Displays Opening Door on the screen*/
	door_status = UART_recieveByte(); /*wait for MC2 reply to move to the closing door phase*/
	LCD_clearScreen(); /*clear screen */
	LCD_displayString("Closing Door"); /*Displays Closing Door on the screen*/
	door_status = UART_recieveByte(); /*wait for MC2 reply know that the door sequence is finished*/
	LCD_clearScreen(); /*clear screen after finishing the process*/
}

void Send_Buzzer_Command(void)
{
	uint8 buzzer_status;
	LCD_clearScreen(); /*clear screen in the start of the process*/
	UART_sendByte(ALERT); /*send alert command to MC2 to start the buzzer*/
	//LCD_displayString("INTRUDER ALERT !"); /*being professional*/
	LCD_displayString("2e2fesh 7araamy"); /*sha2awa*/
	buzzer_status = UART_recieveByte(); /*wait for MC2 reply know that the alert time is finished*/
	LCD_clearScreen(); /*clear screen after finishing the process*/
}

void Display_MainMenu(void)
{
	LCD_clearScreen(); /*clear screen in the start of the process*/
	LCD_displayString("* Change Pass"); /*Displays * Change Pass on the screen*/
	LCD_goToRowColumn(1 , 0); /*moves cursor to the next line*/
	LCD_displayString("# Open Door"); /*Displays # Open Door on the screen*/
}

int main (void)
{
	UartConfig UartSettings = {NO_PARITY,9600,SINGLE,EIGHT_BITS}; /*setting up the uart configuration structure*/
	UART_init(&UartSettings); /*initializing the uart and passing the config struct. to it*/
	LCD_init(); /*initializing lcd*/
	uint8 Flag_Status;
	Flag_Status = Receive_Flag(); /* receiving flag from MC2 telling us if a password exists*/
	switch (Flag_Status)
	{
	case FAIL: ;  /*no password exist (user first time to open the app)*/
		Create_Password(); /*do create password process*/
		Save_Password(); /*send password to MC2 to save it to the eeprom*/
		break;

	case SUCCESS: ; /*password already exists*/
		Receive_Stored_Password(); /*receive the password and store it for future comparisons*/
		break;

	}

	while(1)
	{
		Display_MainMenu(); /*displays main menu with options for the user to choose*/
		uint8 User_Choice;
		User_Choice = KeyPad_getPressedKey(); /*receive the key that the user press to choose from main menu*/
		switch (User_Choice)
		{
		case '*' : /* if user entered *  */
		{
			uint8 Validation_Flag;
			uint8 Wrong_Pass_Counter=0;
			do
			{
				if(Wrong_Pass_Counter > 0)
				{
					LCD_clearScreen(); /*clear screen*/
					LCD_displayString("WRONG PASSWORD"); /* Display WRONG PASSWORD on the screen*/
					_delay_ms(1000); /*delay to give the user time to read*/
					if(Wrong_Pass_Counter >= 3)
					{
						/*
						 * if the user entered a wrong password 3 times the program freezes
						 * send the buzzer command to MC2 to activate the buzzer for a minute
						 * reset Wrong_Pass_Counter to give the user three more tries after the penalty ended
						 */
						Send_Buzzer_Command();
						Wrong_Pass_Counter=0;

					}
				}
				Enter_Password(); /* do the enter password process*/
				Validation_Flag = Validate_Password(); /*compare entered password with stored password*/
				Wrong_Pass_Counter++; /*increase the wrong password counter to use as a flag*/
			}
			while(Validation_Flag == FAIL); /* if the entered password doesn't match the stored password repeat*/

			UART_sendByte(CREATE_PASSWORD); /*send create password flag to MC2 keep the synchronization*/
			Create_Password(); /* do the create password process*/
			Save_Password(); /* send the password to MC2 to save it to the eeprom*/
			break;
		}
		case '#' : /* if user entered # */
		{

			uint8 Validation_Flag;
			uint8 Wrong_Pass_Counter=0;
			do
			{
				if(Wrong_Pass_Counter > 0)
				{
					LCD_clearScreen(); /*clear screen*/
					LCD_displayString("WRONG PASSWORD!"); /* Display WRONG PASSWORD on the screen*/
					_delay_ms(1000); /*delay to give the user time to read*/
					if(Wrong_Pass_Counter >= 3)
					{
						/*
						 * if the user entered a wrong password 3 times the program freezes
						 * send the buzzer command to MC2 to activate the buzzer for a minute
						 * reset Wrong_Pass_Counter to give the user three more tries after the penalty ended
						 */
						Send_Buzzer_Command();
						Wrong_Pass_Counter=0;

					}
				}
				Enter_Password(); /* do the enter password process*/
				Validation_Flag = Validate_Password(); /*compare entered password with stored password*/
				Wrong_Pass_Counter++; /*increase the wrong password counter to use as a flag*/
			}
			while(Validation_Flag == FAIL); /* if the entered password doesn't match the stored password repeat*/
			Send_OpenDoor_Command(); /* do the open door sequence and send open door flag to MC2 to start the door sequence*/
			break;
		}
		default:
			break; /* no respond if the user pressed any other key than * and # */
		}
	}

}
/*ana shayf na3ml el comments de voice notes ba3d kda ashal 34an 7aram walahi XD*/


