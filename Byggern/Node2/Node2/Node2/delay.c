/*
 * delay.c
 *
 * Created: 22.10.2020 15:05:54
 *  Author: jarlenes
 */ 
#include <stdint.h>
#include <sam.h>
//#include <../include/delay.h>
void delay_ms(uint32_t delay) // Millisecond
{
	// SysTick: System timer
	uint32_t ms = 0;
	SysTick -> LOAD = 0x2904; // LOAD: Reload Value Register
	SysTick -> CTRL = 0x1; // CTRL: Control and Status Register
	
	while (ms < delay)
	{
		if (SysTick ->CTRL & (1<<16))
		{
			ms++;
		}
	}
}
void delay_us(uint32_t delay) // Microsecond
{
	uint32_t us = 0;
	SysTick -> LOAD = 0xA; // 0x2904 / 1000 = A
	SysTick -> CTRL = 0x1;
	while (us < delay)
	{
		if (SysTick ->CTRL & (1<<16))
		{
			us++;
		}
	}
}