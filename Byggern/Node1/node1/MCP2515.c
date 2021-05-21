/*
 * MCP2515.c
 *
 * Created: 01.10.2020 10:46:44
 *  Author: jarlenes
 */ 
#define F_CPU 4915200UL
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "MCP2515.h"
#include "SPI_driver.h"
#include <util/delay.h>


// Read data from register beginning at selected address.
uint8_t mcp2515_read(uint8_t address){ 
	SPI_select();
	SPI_send(MCP_READ);
	SPI_send(address);
	uint8_t data = SPI_read();
	SPI_deselect();
	return data;
	
}
// Write data to register beginning at selected address
void mcp2515_write(uint8_t address, uint8_t command){ 
	SPI_select();
	SPI_send(MCP_WRITE);
	SPI_send(address);
	SPI_send(command);
	SPI_deselect();
}
// Request to send
void mcp2515_request_to_send(void){ 
	SPI_select();
	SPI_send(MCP_RTS_TX0);
	SPI_deselect();
}
// Allows the user to set or clear individual bits in a particular register
void mcp2515_bit_modify(uint8_t register_to_modify, uint8_t mask, uint8_t data){ 
	SPI_select();
	SPI_send(MCP_BITMOD);
	SPI_send(register_to_modify);
	SPI_send(mask);
	SPI_send(data);
	SPI_deselect();
}
// Resets internal registers to default state, set Configuration mode
void mcp2515_reset(void){ 
	SPI_select();
	SPI_send(MCP_RESET);
	SPI_deselect();
	_delay_ms(200);
}
// Quick polling command that reads several status bits for transmit and receive functions
void mcp2515_read_status(void){ 
	SPI_select();
	SPI_send(MCP_READ_STATUS);
	uint8_t status = SPI_read();
	SPI_deselect();
	return status;
}