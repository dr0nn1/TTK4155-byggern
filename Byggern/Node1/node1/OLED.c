/*
 * OLED.c
 *
 * Created: 17.09.2020 09:52:51
 *  Author: jarlenes
 */ 
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "OLED.h"
#include "Fonts.h"

void write_c(uint8_t data){  // OLED command
	volatile char* command_address = (char*)0x1000;
	command_address[0] = data;
}
void write_d(uint8_t data){ // OLED data
	volatile char* data_address = (char*)0x1200; 
	data_address[0] = data;
}
void OLED_goto_line(uint8_t line){ // Goes to select line
	OLED_home();
	write_c(0xB0 | line);
	
}
void OLED_clear_line(uint8_t line){ // Clear the line
	OLED_goto_line(line);
	
	for (int col = 0; col < 128; col++) {
		write_d(0x00);
	}
	
}
void OLED_reset(){ // Reset the OLED with clearing the screen
	OLED_home();
	for (int line = 0; line < 8; line++) {
		OLED_clear_line(line);
	}
	
}

void OLED_init(void){
	write_c(0xae); // display off
	write_c(0xa1); //segment remap
	write_c(0xda); //common pads hardware: alternative
	write_c(0x12);
	write_c(0xc8); //common output scan direction:com63~com0
	write_c(0xa8); //multiplex ration mode:63
	write_c(0x3f);
	write_c(0xd5); //display divide ratio/osc. freq. mode
	write_c(0x80);
	write_c(0x81); //contrast control
	write_c(0x50);
	write_c(0xd9); //set pre-charge period
	write_c(0x21);
	write_c(0x20); //Set Memory Addressing Mode
	write_c(0x02);
	write_c(0xdb); //VCOM deselect level mode
	write_c(0x30);
	write_c(0xad); //master configuration
	write_c(0x00);
	write_c(0xa4); //out follows RAM content
	write_c(0xa6); //set normal display
	write_c(0xaf); // display on
	
	write_c(0xB0);
	write_c(0x00);
	write_c(0x10);
	OLED_reset();
	
	
}

void OLED_goto_column(uint8_t column){ // Go to column
	    write_c(0x21);
	    write_c(0x00 | column);
		//write_c(0x10 + column);	
	    write_c(0x7F);
}



void OLED_pos(uint8_t row , uint8_t col ){ // Go to selected position
	OLED_goto_line(row);
	OLED_goto_column(col);
	
}
void OLED_home(){ 
	write_c(0x21);
	write_c(0x00);
	write_c(0x7F);
	write_c(0x22);
	write_c(0x00);
	write_c(0x07);
	
}

void write_char(char c) // Write a char to the OLED
{
	for (uint8_t i =0 ; i<8 ; i++)
	{
		write_d(pgm_read_word(&font8[c-32][i])); //shift linked to ASCII table
	}
}
void OLED_printf(const char *data) // Prints a char or a string to the OLED
{
	while (*data != '\0')
	{
		write_char(*data++);
	}
}




void OLED_print_arrow ( uint8_t row , uint8_t col ) 
{
	OLED_pos ( row , col );
	write_d(0b00011000 );
	write_d(0b00011000 );
	write_d(0b01111110 );
	write_d(0b00111100 );
	write_d(0b00011000 );
}

void OLED_print_flipped_arrow ( uint8_t row , uint8_t col ) // Arrow down
{
	OLED_pos ( row , col );
	write_d(0b00000000);
	write_d(0b00010000);
	write_d(0b00110000);
	write_d(0b01111111);
	write_d(0b01111111);
	write_d(0b00110000);
	write_d(0b00010000);
	write_d(0b00000000);
	
}