/*
* Timer.c
*
* Created: 12.11.2020 15:23:26
*  Author: jarlenes
*/
#define F_CPU 84000000UL
#include <sam.h>
#include <stdio.h>
#include <stdlib.h>

void enableTimer(){
	TC0 -> TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN; // Enable counter clock
	TC0 -> TC_CHANNEL[0].TC_CCR |= TC_CCR_SWTRG; // A software trigger is performed: the counter is reset and the clock is started.
}

void disableTimer(){
	TC0 -> TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS; // Disable counter clock
	
}

void initTimer()
{
	NVIC_EnableIRQ(TC0_IRQn); // Enable IRQ (Interrupt ReQuest)
	
	// Power Management Controller
	PMC -> PMC_PCER0 |= PMC_PCER0_PID27; // Enable TC0 (Timer Counter Channel 0)
	
	// Channel Mode Register
	TC0 -> TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_TIMER_CLOCK1; // Clock selected: internal MCK/2 clock signal (from PMC)
	TC0 -> TC_CHANNEL[0].TC_CMR = TC_CMR_CPCTRG; // If the value is equal the RC(420000) value, then resets the counter and starts the counter clock
	
	TC0 -> TC_CHANNEL[0].TC_RC = 420000; // Frequency = (MCK/2)/TC_RC = 100Hz => Count to 10ms
	
	TC0 -> TC_CHANNEL[0].TC_IER = TC_IER_CPCS; // Enables the RC Compare Interrupt
	
	
	
}
