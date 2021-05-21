/*
 * JoystickAndSlider.h
 *
 * Created: 10.09.2020 13:28:24
 *  Author: jarlenes
 */ 


#ifndef JOYSTICKANDSLIDER_H_
#define JOYSTICKANDSLIDER_H_

#include <avr/io.h>

typedef enum
{
	LEFT ,
	RIGHT ,
	UP ,
	DOWN ,
	NETRUAL
		
} direction_joy;

typedef struct  
{
	uint8_t dir_x;
	uint8_t dir_y;
} joy_pos;

uint8_t map(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max);
void joystickAndSlider_init();
joy_pos get_joystick_pos();
joy_pos get_joystick_percent();
uint8_t get_joystick_dir();
uint8_t get_left_slider();
uint8_t get_right_slider();
void joyStickTest();




#endif /* JOYSTICKANDSLIDER_H_ */