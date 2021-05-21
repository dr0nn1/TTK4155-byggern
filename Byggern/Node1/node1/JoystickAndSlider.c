/*
 * JoystickAndSlider.c
 *
 * Created: 10.09.2020 13:39:54
 *  Author: jarlenes
 */ 
#define F_CPU 4915200UL
#include "JoystickAndSlider.h"
#include "adc_driver.h"
#include <util/delay.h>

uint16_t neutralX;
uint16_t neutralY;
int8_t deadzone = 10;


uint8_t map(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max) 
{
	return ((x - in_min) * (out_max - out_min) / (in_max - in_min)) + out_min;
}

void joystickAndSlider_init(){	
	for (uint8_t i = 0; i < 5; i++)
	{
		neutralX += adc_read(X_axis);
		neutralY += adc_read(Y_axis);
	}
	
	neutralX = neutralX/5;
	neutralY = neutralY/5; 
}


joy_pos get_joystick_pos(){
	joy_pos  joy_position;
	joy_position.dir_x = adc_read(X_axis);
	joy_position.dir_y = adc_read(Y_axis);
	
	return joy_position;
}
joy_pos get_joystick_percent(){
	joy_pos  joy_perc = get_joystick_pos();
	
	if(joy_perc.dir_x >= 160 & joy_perc.dir_x <= 170)
	{
		joy_perc.dir_x = 50;	
	}
	else if (joy_perc.dir_x < 160)
	{
		joy_perc.dir_x = map(joy_perc.dir_x,0,160,0,49);
	}
	else if (joy_perc.dir_x > 170)
	{
		joy_perc.dir_x = map(joy_perc.dir_x,170,255,51,100);
	}
	if(joy_perc.dir_y >= 165 & joy_perc.dir_y <= 175)
	{
		joy_perc.dir_y = 50;
	}
	else if (joy_perc.dir_y < 165)
	{
		joy_perc.dir_y = map(joy_perc.dir_y,0,165,0,49);
	}
	else if (joy_perc.dir_y > 175)
	{
		joy_perc.dir_y = map(joy_perc.dir_y,175,255,51,100);
	}
	
	return joy_perc;
	
}

uint8_t get_joystick_dir(){
	joy_pos joy_position = get_joystick_pos();
	uint8_t ypos = joy_position.dir_y;
	uint8_t direction;
	if (joy_position.dir_y > 200)
	{
		direction = UP;
	}
	
	else if (joy_position.dir_y < 120)
	{
		direction =  DOWN;
	}
	else if (joy_position.dir_x > 200)
	{
		direction =  RIGHT;
	}	

	else if (joy_position.dir_x < 120)
	{
		direction =  LEFT;
	}
	
	else 
	{
		direction =  NETRUAL;
	}
	return direction;
}

uint8_t get_left_slider(){
	return adc_read(L_slider);
}

uint8_t get_right_slider(){
	return adc_read(R_slider);
}

void joyStickTest(){
	joy_pos joyStick = get_joystick_pos();
	uint8_t direction = get_joystick_dir();

	printf("X value is: ");
	printf("%d\n\r", (joyStick.dir_x));
	printf("Y value is: ");
	printf("%d\n\r", (joyStick.dir_y));
	if (direction == UP)
	{
		printf("Direction is UP \n\r");
	}
		else if (direction == DOWN)
	{
		printf("Direction is DOWN \n\r");
	}
			else if (direction == LEFT)
	{
		printf("Direction is LEFT \n\r");
	}
			else if (direction == RIGHT)
	{
		printf("Direction is RIGHT \n\r");
	}
			else
	{
		printf("Direction is NEUTRUAL \n\r");
	}
		
	printf("\n Left slider value is: ");
	printf("%d\n\r", get_left_slider());

	printf("Right slider value is: ");
	printf("%d\n\r", get_right_slider());
	_delay_ms(500);


}
