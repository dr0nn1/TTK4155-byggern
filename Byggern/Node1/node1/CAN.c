/*
* CAN.c
*
* Created: 01.10.2020 11:55:29
*  Author: jarlenes
*/
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "CAN.h"
#include "MCP2515.h"
#include "menu.h"

void CAN_init(void){
	mcp2515_write(MCP_CANCTRL, MODE_CONFIG); // Configuration mode
	// Control the bit timing for the CAN bus interface
	mcp2515_bit_modify(MCP_CNF3,0x07, 0b00000111); // Baud Rate Prescaler, Tq = 2*(BRP+1)/Fosc. BRP = 7
	mcp2515_bit_modify(MCP_CNF2,0xFF, 0b10101100); // The SAM bit controls how many times the RXCAN pin is sampled
	mcp2515_bit_modify(MCP_CNF1,0xFF ,0b00000011); // PHSEG2 (PS2 length bits), (PHSEG2 + 1) * Tq. PHSEG2 = 3
	
	mcp2515_write(MCP_CANCTRL, MODE_NORMAL); // Normal mode
	CAN_message_interrupt_init();
}
void CAN_send(can_message* msg ) {
	mcp2515_bit_modify(MCP_TXB0CTRL, 0xFF, 0x00); // Sets the priority of the message to 0
	
	mcp2515_write(MCP_SID0_HIGH,msg ->id >> 3); // Transmit buffer
	mcp2515_write(MCP_SID0_LOW,msg ->id << 5); // Transmit buffer
	mcp2515_write(MCP_TXB0DLC,(msg->data_length)& (0x0F) ); // Transmit buffer
	
	for (uint8_t  i = 0; i < msg->data_length; i ++){
		mcp2515_write(MCP_TXB + i, msg -> data[i]);  // Write to the buffer
	}
	mcp2515_request_to_send(); 
}

void CAN_reset_interrupt(){
	mcp2515_bit_modify(MCP_CANINTF, 0xFF, 0x00); // Rest the flag
}

can_message CAN_recieve(void){
	can_message message;

	message.id = (mcp2515_read(MCP_SID0_HIGH) << 3) | (mcp2515_read(MCP_SID0_LOW) >> 5); // Receive buffer
	message.data_length = (mcp2515_read(MCP_RXB0DLC)) & (MCP_DATA_LENGTH_BITS); // Receive buffer


	for(uint8_t i = 0; i < message.data_length; i++){
		message.data[i] = mcp2515_read(MCP_RXB + i); // Read the buffer
	}
	CAN_reset_interrupt();
	return message;
}



void CAN_message_interrupt_init(void){
	mcp2515_bit_modify(MCP_CANINTE, 0xFF, 0xFF); // Interrupt enable
	write_bit(1, GICR, INT0);
	write_bit(0, MCUCR, ISC00);
	write_bit(0, MCUCR, ISC01);
	sei(); // Enables interrupts by setting the global interrupt mask
}
ISR(INT0_vect) // Interrupt service routine, INT0_vect = external interrupt 0
{
	uint8_t flag = mcp2515_read(MCP_CANINTF); // Check if a message has arrived in the receive buffer

	if (flag)
	{
		can_message node2 = CAN_recieve(); // Receive message 
		if(node2.id == 2) updateScoreboard(node2.data[0]); // Update scoreboard if received message has the same ID as sent from node 2
	}
	
}