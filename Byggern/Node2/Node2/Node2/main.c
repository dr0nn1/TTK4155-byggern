/*
 * Node2.c
 *
 * Created: 08.10.2020 10:24:11
 * Author : jarlenes
 */ 
#include <sam.h>
#include <stdio.h>
#include <stdlib.h>

#include "can_controller.h"
#include "can_interrupt.h"
#include "printf-stdarg.h"
#include "PWM.h"
#include "delay.h"
#include "IR.h"
#include "MotorControl.h"
#include "uart.h"
#include "Timer.h"
int main(void)
{   
	/* Initialize the SAM system */
	SystemInit();
	configure_uart();
	printf("Node 2 starting up!\n\r");
	pwm_init();
	initIR();
	InitMotorControl();
	initSolenoid();
	CAN_MESSAGE msg;
	initTimer();
	  
	WDT ->WDT_MR = WDT_MR_WDDIS; // Watchdog Timer Mode Register, WDDIS: Disable the Watchdog timer
	uint32_t can_br = 0x290457; //param can_br Value to be set in CAN0->CAN_BR register to match can bus bit timing
	can_init_def_tx_rx_mb(can_br);
	
	//Sends a clean score message to node 1
	resetScore(); 
	sendScore();
	
}
