/*
 * CAN.h
 *
 * Created: 01.10.2020 11:55:44
 *  Author: jarlenes
 */ 


#ifndef CAN_H_
#define CAN_H_

#define MCP_SID0_HIGH   0x31
#define MCP_SID0_LOW    0x32
#define MCP_RXB0DLC     0x65
#define MCP_RXB1DLC     0x75
#define MCP_TXB0DLC     0x35
#define MCP_TXB1DLC     0x45
#define MCP_RXB         0x66
#define MCP_TXB         0x36

#define MCP_TX_PRI_BITS          0b00000011
#define MCP_DATA_LENGTH_BITS     0b00001111
#define MCP_RX_BUFF_OP_MODE_BITS 0b01100000
#define MCP_MSG_LOST_ARB_BIT     5
#define MCP_TX_ERR_DETECTED_BIT  4
#define MCP_MSG_TX_REQ_BIT       3
#define MCP_RX_BUFF0_FULL_FLAG   0b00000001
#define MCP_RX_BUFF0_FULL_ENABLE 0b00000001

typedef struct{
	uint8_t id ;
	uint8_t data_length ;
	uint8_t data [8];
}  can_message ;



void CAN_init(void);
void CAN_send(can_message* msg );
can_message CAN_recieve(void);
void CAN_message_interrupt_init();


#endif /* CAN_H_ */