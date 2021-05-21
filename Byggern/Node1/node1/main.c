/*
 * Lab1.c
 *
 * Created: 27.08.2020 08:32:09
 * Author : jarlenes
 */ 
#define F_CPU 4915200UL
#include <avr/io.h>
#include "UARTdriver.h"
#include "SRAMtest.h"
#define FOSC 4915200UL // Clock Speed
#define BAUD 9600
#define MYUBRR 31
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "JoystickAndSlider.h"
#include "adc_driver.h"
#define PWM_TOP 0x05
#include "OLED.h"
#include "Menu.h"
#include "SPI_driver.h"
#include "MCP2515.h"
#include "CAN.h"

// | = BitWise OR
void pwm_init() 
{
		// MCU Control Register 
		MCUCR |= (1<<SRE); // Enables the external memory interface
		// Special Function IO Register
		SFIOR |= (1<<XMM2); // When the External Memory is enabled, all Port C pins are used for the high address byte by default. Released port pins PC7-PC4
	
	cli(); // Disables all interrupts by clearing the global interrupt mask
	
	DDRB |= (1<< PB0); // Port B Data Direction Register
	
	TCCR0 = (1<< COM01) | (1<< WGM11); // Timer/Counter Control Register, (1<< WGM11): Fast PWM, (1<< COM01): Reserved
	TCCR0 = (1<< CS10) | (1<<WGM12) | (1<<WGM13); // (1<< CS10): No prescaling,  (1<<WGM12) | (1<<WGM13): CTC
	
	TIMSK = (1 << TOIE0); // Overflow Interrupt Enable
	
	OCR0 = PWM_TOP / 2; // The Output Compare Register contains an 8-bit value that is continuously compared with thecounter value (TCNT0). A match can be used to generate an output compare interrupt, or to generate a waveform output on the OC0 pin.
	ICR1 = PWM_TOP; // The Input Capture is updated with the counter (TCNTn) value each time an event occurs on the ICPn pin (or optionally on the Analog Comparator output for Timer/Counter1).
	
	sei(); // Enables all interrupts by clearing the global interrupt mask
}


int main(void){	
	initMenu();
	UART_Init(MYUBRR); // MYUBRR = UBRR. The USART Baud Rate Register (UBRR), baudrate = fosc/(UBRR+1))
	pwm_init();
	joystickAndSlider_init();
	OLED_init();
	SPI_init();
	printf("\nNode 1 starting up!\n\r");
	
	CAN_init();
	
	//Starting the main menu of the game
	menu_main();
	
	return 0;
}


