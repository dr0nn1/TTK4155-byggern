#define F_CPU 4915200UL
#include "UARTdriver.h"
#include <avr/io.h>
#include <stdio.h>


int UART_Transmit(char data);
int UART_Receive(void);

void UART_Init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L= (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	//* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);
	fdevopen(UART_Transmit, UART_Receive);
}

int UART_Transmit(char data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0))){};
	/* Put data into buffer, sends the data */
	UDR0 = data;
	return 0;
}
int UART_Receive(void){
	
	while (!(UCSR0A & (1<<RXC0)));
	return (int)UDR0;
}
