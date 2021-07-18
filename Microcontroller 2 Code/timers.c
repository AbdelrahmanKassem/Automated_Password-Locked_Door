  /******************************************************************************
 *
 * Module:TIMERS - Driver
 *
 * File Name: timers.c
 *
 * Description: General driver used to operate timers in AVR micro-controller
 *
 * Author: Kassem
 *
 *******************************************************************************/


#include "timers.h"

/*
 * global variables to hold the address of the call back function in the application
 * making them volatile to avoid compiler optimizations
 * static as they shouldn't be used outside this file
 */
static volatile void (*g_timer0CallBackPtr)(void) = NULL;
static volatile void (*g_timer1CallBackPtr)(void) = NULL;
static volatile void (*g_timer1CompBCallBackPtr)(void) = NULL;
static volatile void (*g_timer2CallBackPtr)(void) = NULL;


ISR (TIMER0_OVF_vect) /*timer 0 overflow mode interrupt*/
{
	if(g_timer0CallBackPtr != NULL) /*checking if the pointer to function is set before calling it*/
	{
		/* calling the Call Back function in the application when receiving interrupt */
		g_timer0CallBackPtr();
	}
}

ISR (TIMER0_COMP_vect) /*timer 0 compare mode interrupt*/
{
	if(g_timer0CallBackPtr != NULL)/*checking if the pointer to function is set before calling it*/
	{
		/* calling the Call Back function in the application when receiving interrupt */
		g_timer0CallBackPtr();
	}
}

ISR (TIMER1_OVF_vect) /*timer 1 overflow mode interrupt*/
{
	if(g_timer1CallBackPtr != NULL)/*checking if the pointer to function is set before calling it*/
	{
		/* calling the Call Back function in the application when receiving interrupt */
		g_timer1CallBackPtr();
	}
}

ISR (TIMER1_COMPA_vect) /*timer 1 compare mode channel a interrupt*/
{
	if(g_timer1CallBackPtr != NULL)/*checking if the pointer to function is set before calling it*/
	{
		/* calling the Call Back function in the application when receiving interrupt */
		g_timer1CallBackPtr();
	}
}

ISR (TIMER1_COMPB_vect) /*timer 1 compare mode channel b interrupt*/
{
	if(g_timer1CompBCallBackPtr != NULL)/*checking if the pointer to function is set before calling it*/
	{
		/* calling the Call Back function in the application when receiving interrupt */
		g_timer1CompBCallBackPtr();
	}
}


ISR (TIMER2_OVF_vect) /*timer 2 overflow mode interrupt*/
{
	if(g_timer2CallBackPtr != NULL)/*checking if the pointer to function is set before calling it*/
	{
		/* calling the Call Back function in the application when receiving interrupt */
		g_timer2CallBackPtr();
	}
}

ISR (TIMER2_COMP_vect) /*timer 2 compare mode interrupt*/
{
	if(g_timer2CallBackPtr != NULL)/*checking if the pointer to function is set before calling it*/
	{
		/* calling the Call Back function in the application when receiving interrupt */
		g_timer2CallBackPtr();
	}
}



void Timer_init(const Timer_ConfigType *a_TimerConfig_Ptr)
{
	/*
	 * function receives the configuration structure
	 * initializes timer registors according to the received structure
	 */
	switch (a_TimerConfig_Ptr->s_timer) /*determine which timer the user want to configure*/
	{
	case TIMER_0:
		if(a_TimerConfig_Ptr->s_mode == OVERFLOW) /*configure timer 0 overflow mode*/
		{
			TCCR0 =  (TCCR0 & 0xF8) | (a_TimerConfig_Ptr->s_prescaler); /*set timer prescaler specified by the user in the config struct*/
			TCCR0 = (TCCR0 & 0X07) | (1<<FOC0);
			TCNT0 = a_TimerConfig_Ptr->s_initValue; /*set timer initial value specified by the user in the config struct*/
			TIMSK |= (1<<TOIE0);/*enable overflow interrupt*/
		}
		else if(a_TimerConfig_Ptr->s_mode == COMPARE_MATCH) /*configure timer 0 compare mode*/
		{
			TCCR0 =  (TCCR0 & 0xF8) | (a_TimerConfig_Ptr->s_prescaler); /*set timer prescaler specified by the user in the config struct*/
			TCCR0 = (TCCR0 & 0X07) | (1<<WGM01) | (1<<FOC0);
			TCNT0 = a_TimerConfig_Ptr->s_initValue; /*set timer initial value specified by the user in the config struct*/
			OCR0 = a_TimerConfig_Ptr->s_compareValue; /*set timer compare value specified by the user in the config struct*/
			TIMSK |= (1<<OCIE0);/*enable compare interrupt*/
		}
		break;
	case TIMER_1:
		if(a_TimerConfig_Ptr->s_mode == OVERFLOW) /*configure timer 2 overflow mode*/
		{
			TCCR1A = (1<<FOC1A) | (1<<FOC1B);
			TCCR1B = (TCCR1B & 0xF8) | (a_TimerConfig_Ptr->s_prescaler); /*set timer prescaler specified by the user in the config struct*/
			TCCR1B = (TCCR1B & 0X07);
			TCNT1 = a_TimerConfig_Ptr->s_initValue; /*set timer initial value specified by the user in the config struct*/
			TIMSK = (1<<TOIE1);/*enable overflow interrupt*/
		}
		else if(a_TimerConfig_Ptr->s_mode == COMPARE_MATCH) /*configure timer 1 compare mode*/
		{
			TCCR1A = (1<<FOC1A) | (1<<FOC1B);
			TCCR1B = (TCCR1B & 0xF8) | (a_TimerConfig_Ptr->s_prescaler); /*set timer prescaler specified by the user in the config struct*/
			TCCR1B = (TCCR1B & 0X07) | (1<<WGM12);
			TCNT1 = a_TimerConfig_Ptr->s_initValue; /*set timer initial value specified by the user in the config struct*/
			OCR1A = a_TimerConfig_Ptr->s_compareValue; /*set timer compare value in channel A specified by the user in the config struct*/
			OCR1B = a_TimerConfig_Ptr->s_compareValue_B; /*set timer compare value in channel B specified by the user in the config struct*/
			TIMSK = (1<<OCIE1A) | (1<<OCIE1B);/*enable compare interrupt of channel A and B*/
		}
		break;
	case TIMER_2:
		if(a_TimerConfig_Ptr->s_mode == OVERFLOW) /*configure timer 2 overflow mode*/
		{
			TCCR2 =  (TCCR2 & 0xF8) | (a_TimerConfig_Ptr->s_prescaler); /*set timer prescaler specified by the user in the config struct*/
			TCCR2 = (TCCR2 & 0X07) | (1<<FOC2);
			TCNT2 = a_TimerConfig_Ptr->s_initValue; /*set timer initial value specified by the user in the config struct*/
			TIMSK = (1<<TOIE2);/*enable overflow interrupt*/
		}
		else if(a_TimerConfig_Ptr->s_mode == COMPARE_MATCH) /*configure timer 2 compare mode*/
		{
			TCCR2 =  (TCCR2 & 0xF8) | (a_TimerConfig_Ptr->s_prescaler); /*set timer prescaler specified by the user in the config struct*/
			TCCR2 = (TCCR2 & 0X07)  | (1<<WGM21) | (1<<FOC2);
			TCNT2 = a_TimerConfig_Ptr->s_initValue; /*set timer initial value specified by the user in the config struct*/
			OCR2 =  a_TimerConfig_Ptr->s_compareValue; /*set timer compare value specified by the user in the config struct*/
			TIMSK = (1<<OCIE2);/*enable compare interrupt*/
		}
		break;
	}
}

void Timer_setCallBack(Timer_Number a_Timer,void(*a_Ptr)(void))
{
	/*
	 * function receives a variable which determines the timer requested by the user
	 * the function also receives the address to the call back function in the application which is configured for it
	 */
	switch (a_Timer) /*choose which timer to save the call back address to according to the application*/
	{
	case TIMER_0:
		g_timer0CallBackPtr = a_Ptr; /* Save the address of the Call back function in the timer 0 global variable */
		break;
	case TIMER_1:
		g_timer1CallBackPtr = a_Ptr; /* Save the address of the Call back function in the timer 1 global variable */
		break;
	case TIMER_2:
		g_timer2CallBackPtr = a_Ptr; /* Save the address of the Call back function in the timer 2 global variable */
		break;
	}
}

void Timer_1_CompB_setCallBack(void(*a_Ptr)(void))
{
	/*function for timer 1 channel b in compare mode receives a variable which determines the timer requested by the user*/
	g_timer1CompBCallBackPtr = a_Ptr; /* Save the address of the Call back function in the timer 1 channel b global variable */
}

uint16 Timer_getTimerValue(Timer_Number a_Timer)
{
	/*
	 * function to return the current value of the timer
	 * function receives a variable which determines the timer requested by the user
	 */
	switch (a_Timer)
	{
	case TIMER_0:
		return TCNT0; /*return the value of timer 0 to the user*/
		break;
	case TIMER_1:
		return TCNT1; /*return the value of timer 1 to the user*/
		break;
	case TIMER_2:
		return TCNT2; /*return the value of timer 2 to the user*/
		break;
	default:
		return 0;
	}
}

void Timer_clearTimerValue(Timer_Number a_Timer)
{
	/*
	 * function to reinitialize the current value of the requested timer to zero
	 * function receives a variable which determines the timer requested by the user
	 */
	switch (a_Timer)
	{
	case TIMER_0:
		TCNT0 = 0; /*initializing value of timer 0 to zero*/
		break;
	case TIMER_1:
		TCNT1 = 0; /*initializing value of timer 1 to zero*/
		break;
	case TIMER_2:
		TCNT2 = 0; /*initializing value of timer 2 to zero*/
		break;
	}
}
void Timer_deInit(Timer_Number a_Timer)
{
	/*
	 * function to deinitialize (disable) the requested timer
	 * function receives a variable which determines the timer requested by the user
	 */
	switch (a_Timer)
	{
	case TIMER_0:
		/*
		 * clearing all the registers in timer 0
		 * disable all the timer interrupts
		 */
		TCCR0 = 0;
		TCNT0 = 0;
		OCR0 = 0;
		TIMSK &= ~(1<<OCIE0) & ~(1<<TOIE0);
		break;
	case TIMER_1:
		/*
		 * clearing all the registers in timer 1
		 * disable all the timer interrupts
		 */
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
		OCR1A = 0;
		OCR1B = 0;
		TIMSK &= ~(1<<OCIE1A) & ~(1<<OCIE1B) & ~(1<<TOIE1);
		break;
	case TIMER_2:
		/*
		 * clearing all the registers in timer 2
		 * disable all the timer interrupts
		 */
		TCCR2 = 0;
		TCNT2 = 0;
		OCR2 = 0;
		TIMSK &= ~(1<<OCIE2) & ~(1<<TOIE2);
		break;
	}
}
