/*
* adc_driver.c
*
* Created: 10.09.2020 14:55:29
*  Author: jarlenes
*/

#define F_CPU 4915200UL
#include "adc_driver.h"
#include <util/delay.h>

#define ADC_ADDRESS 0x1400 // Address to ADC

volatile char*ext_adc = ADC_ADDRESS; // Make a pointer to the array that contains all the addresses that we write to


uint8_t adc_read(uint8_t channel)
{
	ext_adc[0x00] = 0b10000000 | channel; // Choose channel

	_delay_ms(10);
	
	return ext_adc[0];
}