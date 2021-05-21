#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>

void xmem_write(uint8_t data, uint16_t addr){
	int BASE_ADDRESS = 0x1800;
	volatile char *ext_mem = (char* ) BASE_ADDRESS;
	ext_mem[addr] = data;
}
