/*
* IR.c
*
* Created: 29.10.2020 11:34:07
*  Author: jarlenes
*/
#include <sam.h>
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
#include "can_controller.h"

// Global variables
uint8_t sent = 0;
uint8_t score = 0;

void initIR(){
	PMC -> PMC_PCER1 |= PMC_PCER1_PID37; //For PMC Peripheral Clock enabler Register 1: We use ID-37 for ADC controller
	
	ADC -> ADC_MR |= ADC_MR_FREERUN_ON; // Single ended free run mode is the easiest according to the lab lecture

	// Enable channel
	ADC -> ADC_CHER |= ADC_CHER_CH7; // Choose channel 0, AD0 is the pin for CH0
}


uint32_t readIR()
{
	// Start conversion
	ADC -> ADC_CR |= ADC_CR_START;
	
	while (!(ADC->ADC_ISR)); // Wait for conversion to finish
	return ADC -> ADC_CDR[7]; // Reads channel 0
}


void resetScore(){ // Reset score
	score =0;
}

void sendScore(){
	// Creates a CAN message and sends it to node 1
	CAN_MESSAGE message;
	message.id = 1;
	message.data_length = 1;
	message.data[0] = score;
	can_send(&message,0);
}


void goalScored(){
	// If ball come close enough it scores, the length of the goal zone is 1000
	if (readIR() < 1000 & sent == 0 )
	{
		score++; // Update the score
		sent = 1;
		sendScore(); // Sends the score to node 1
	}
	
	else if (readIR() > 1000 & sent ==1) // Wait for the ball to get out of the goal zone, before allowing to score a new goal
	{
		sent = 0;
	}
	
}






