/*
 * SPI_driver.c
 *
 * Created: 01.10.2020 10:15:28
 *  Author: jarlenes
 */ 
#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>
#define SPI_PORT_DDR DDRB
#define SPI_PORT PORTB
#define SPI_SS   PB4
#define SPI_MOSI PB5
#define SPI_MISO PB6
#define SPI_SCK  PB7

void SPI_init(void) {
	//Set MOSI, SCK and SS as output pins
	SPI_PORT_DDR |= (1<<SPI_MOSI) | (1<<SPI_SCK) | (1<<SPI_SS);

	//Set MISO as input pin
	SPI_PORT_DDR &= ~(1<<SPI_MISO);
	//Enable SPI in master mode and set clock rate fosc/16
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);

	//printf("SPI initialized\r\n");
}
void SPI_send(uint8_t data_byte) {
	//Start the transmission
	SPDR = data_byte;

	//Wait for data to be transmitted (checks if the register is empty)
	while(!(SPSR & (1<<SPIF)));
}

uint8_t SPI_read(void) {
	//Send dummy data to read from slave
	SPI_send(0);
	//Wait for data to be received
	while(!(SPSR & (1<<SPIF)));

	return SPDR;
}
void SPI_select(void){
//Set !SS to 0 to select the slave
SPI_PORT &= ~(1<<SPI_SS);
}

void SPI_deselect(void) {
	//Set !SS to 1 to deselect the slave
	SPI_PORT |= (1<<SPI_SS);
}

void SPI_test(void){
	SPI_select();
	SPI_send(0xaa);
	SPI_deselect();
}
