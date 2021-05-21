/*
* menu.c
*
* Created: 24.09.2020 12:24:48
*  Author: jarlenes
*/

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#define F_CPU 4915200UL
#include <util/delay.h>
#include "menu.h"
#include "JoystickAndSlider.h"
#include "OLED.h"
#include "CAN.h"
#include "MCP2515.h"

char menu_arraymain[5][18] = { "New game", "Scoreboard","Create name","Select player","Reset score"}; // The main menu
char names[3][7] = {"Petter","Jarle","Johan"}; // Set this names as default, when starting up the game.
int score[3] = {0,0,0}; // An array of the score
int8_t Player = 0; // Target the current player
void initMenu()
{
	DDRB &= ~(1<<PINB2 | 1<<PINB3); // Init: sets pin 2 and 3 as inputs
}

void menu_print(uint8_t arrow)
{
	OLED_pos(0,0);
	OLED_reset();
	OLED_print_arrow(arrow,0); // An arrow that shows where in the main menu you are
	
	// Prints the main menu
	for (uint8_t i=0; i<5;i++) 
	{
		OLED_pos(i,10); 
		OLED_printf(menu_arraymain[i]);
	}

}



void createName(){
	OLED_pos(0,0);
	OLED_reset();
	char posArray[] = {'A','A','A','A'}; // The default letters
	int8_t pos = 0;
	_Bool execute = false;
	uint8_t rightValue = 0;
	
	while(!rightValue){
		rightValue = (PINB & (1 << PINB2)) >> PINB2; // Reads the value of the right pin
		uint8_t direction = get_joystick_dir(); // Finds out what direction the joystick is in
		OLED_pos(0,0);
		OLED_print_flipped_arrow(2,(5+pos)*8); // An arrow that follows what letter you are selecting
		OLED_pos(0,0);
		for (int i=0;i<4;i++)
		{
			OLED_pos(3,((5+i)*8)); // Change the position for the new letter
			write_char(posArray[i]); // Prints the four letters on the OLED
		}
		
		
		
		if(direction == NETRUAL){
			execute = true;
		}
		
		if(execute){
			
			if (direction == RIGHT)
			{
				pos++;
				if (pos > 3) // If you scroll past the last letter(to right), you start on the first letter
				{
					pos = 0;
				}
				OLED_reset();
				execute = false;
			}
			
			
			else if (direction == LEFT)
			{
				pos--;
				if (pos < 0) // If you scroll past the first letter(to left), you start on the last letter
				{
					pos = 3;
				}
				OLED_reset();
				execute = false;
			}
			
			// ASCII code for A is 65
			// ASCII code for Z is 90
			else if (direction == UP)
			{
				posArray[pos] -= 1;
				if (posArray[pos] < 65) // If you scroll past letter A, then you get Z
				{
					posArray[pos] = 90; // Z
					
				}
				execute = false;
			}
			
			else if (direction == DOWN) // If you scroll past letter Z, then you get A
			{
				posArray[pos] += 1;
				if (posArray[pos] > 90)
				{
					posArray[pos] = 65; // A
				}
				execute = false;
			}
		}
	}
	char name[] = "    ";

	for (int i = 0; i<4; i++) // Makes a string out of the four letters
	{
		name[i] = posArray[i];
	}
	strcpy(names[Player],name); // Copy the string into the names array

	menu_main(); // Return to main menu
}


//



void menu_main()
{
	uint8_t menu_choice = menu_navigate(); // Changes the menu choice based on the joystick 
	switch (menu_choice)
	{
		case 0:
		Newgame();
		break;
		case 1:
		Scoreboard();
		break;
		case 2:
		createName();
		break;
		case 3:
		selectPlayer();
		break;
		case 4:
		resetScore();
		break;
		default:
		menu_main();
		break;
	}
}

uint8_t menu_navigate()
{
	uint8_t menupos = 0;
	_Bool execute = true;
	menu_print(menupos);
	bool running = true;
	while (running)
	{
		uint8_t direction = get_joystick_dir();
		
		if (direction == NETRUAL)
		{
			execute = true;
		}
		
		if (execute)
		{
			
			switch (direction)
			{
				
				case LEFT:
				execute = false;
				break;
				
				case UP: 
				if (menupos==0) // If you scroll past the first line of the menu, then you start at the last line
				{
					menupos=4 ;
				}
				else menupos--; // Up in the menu
				menu_print(menupos);
				
				execute = false;
				
				break;
				
				case DOWN: 
				if (menupos == 4) // If you scroll past the last line of the menu, then you start at the first line
				{
					menupos = 0;
				}
				else menupos++; // Down in the menu
				menu_print(menupos);
				execute = false;
				break;
				
				case RIGHT: // Goes into the select mode
				execute = false;
				running = false;
				
				break;
				
				case NETRUAL:
				break;
				
				default:
				OLED_reset();
				OLED_printf("error");
				break;
				
			}
			
		}
	}
	return menupos;

}

void Newgame()
{
	OLED_reset();
	OLED_pos(1,30);
	OLED_printf("Playing..");
	OLED_pos(3,30);
	OLED_printf(names[Player]);
	OLED_pos(5,10);
	OLED_printf("Highscore: ");
	char buffer[10];

	
	
	
	uint8_t rightValue = 0;
	joy_pos position;
	// Creates a CAN message and sends it to node 2
	can_message message;
	message.id = 2;
	message.data_length = 6;
	message.data[4] = 1; // When node2 receive data[4] = 1 it starts
	
	while(!rightValue)
	{
		itoa(score[Player],buffer,10); // Convert int into string
		OLED_pos(5,90);
		OLED_printf(buffer); // Prints highscore
		rightValue = (PINB & (1 << PINB2)) >> PINB2; // Reads the right button
		position = get_joystick_percent(); // Find the joystick postion of x and y
		message.data[0] = position.dir_x; // Pos x
		message.data[1] = position.dir_y; // Pos y
		message.data[2] = get_left_slider(); // Left slider value
		message.data[3] = rightValue; // Right button
		message.data[5] = (PINB & (1 << PINB3)) >> PINB3; // Left button
		CAN_send(&message); // Sends it to node 2
		message.data[4] = 0; // Reset the value
		_delay_ms(100);
	}
	
	menu_main();
}

void Scoreboard()
{
	OLED_reset();
	OLED_pos(0,0);
	
	
	for (int i = 0; i<3; i++) // Prints the names of the players
	{
		OLED_pos(i,20);
		OLED_printf(names[i]);
	}
	
	char buffer[10];
	for (int i = 0; i<3; i++) // Prints the scores of the players
	{
		itoa(score[i],buffer,10);
		OLED_pos(i,90);
		OLED_printf(buffer);
	}
	
	uint8_t rightValue = 0;
	while(!rightValue) // Return to main menu if right button is pressed
	{
		rightValue = (PINB & (1 << PINB2)) >> PINB2;
		
	}
	
	menu_main();
}
void updateScoreboard(int newScore) // Check if the new score is a new highscore
{
	if (newScore > score[Player]){
		
		score[Player]=newScore;
}

}

void selectPlayer(){
	OLED_reset();
	OLED_pos(0,0);

	_Bool execute = false;
	uint8_t rightValue = 0;
	
	while(!rightValue){ // Select player when you press the joystick to the right
		rightValue = (PINB & (1 << PINB2)) >> PINB2;
		uint8_t direction = get_joystick_dir();
		OLED_print_arrow(Player,8);
		for (int i=0;i<3;i++) // Prints the name of the players you can choose
		{
			OLED_pos(i,30);
			OLED_printf(names[i]);
			
		}
		
		if(direction == NETRUAL){
			execute = true;
		}
		
		if(execute){
			
			if (direction == DOWN)
			{
				Player++;
				if (Player > 2) // If you scroll past the last player, then you start at the first player
				{
					Player = 0; // First
				}
				OLED_reset();
				execute = false;
			}
			
			
			else if (direction == UP)
			{
				Player--;
				if (Player < 0)// If you scroll past the first player, then you start at the last player
				{
					Player = 2; // Last
				}
				OLED_reset();
				execute = false;
			}
		}
	}
	menu_main(); // Return to main menu
}


void resetScore(){ // Reset the score in the scoreboard
	OLED_reset();
	OLED_pos(2,10);
	OLED_printf("Reseting score");
	for (int i=0; i<3;i++)
	{
		score[i] =0;
	}
	_delay_ms(500);
	menu_main();
}