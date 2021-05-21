/*
 * OLED.h
 *
 * Created: 17.09.2020 09:53:05
 *  Author: jarlenes
 */ 


#ifndef OLED_H_
#define OLED_H_

void OLED_init(void);
void OLED_reset();
void OLED_home();
void OLED_goto_line(uint8_t line);
void OLED_goto_column(uint8_t column);
void OLED_clear_line(uint8_t line);
void OLED_pos(uint8_t row , uint8_t col);
void OLED_print(char* a);
void OLED_clearPage(uint8_t page);
void OLED_GoToPage(uint8_t page);
void write_char(char c);
void OLED_printf(const char *data);
void OLED_print_arrow ( uint8_t row , uint8_t col );
void OLED_print_flipped_arrow ( uint8_t row , uint8_t col );

#endif /* OLED_H_ */