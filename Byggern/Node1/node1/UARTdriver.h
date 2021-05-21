#ifndef UARTDRIVER_H_
#define UARTDRIVER_H_ 


int UART_Transmit(char data);
int UART_Receive(void);
void UART_Init(unsigned int ubrr);
#endif  //UARTDRIVER_H_