/*
 * adc_driver.h
 *
 * Created: 10.09.2020 14:54:54
 *  Author: jarlenes
 */ 


#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include <stdint.h>

enum channel_type{
	Y_axis , //channel 0
	X_axis , //channel 1
	L_slider , //channel 2
	R_slider  //channel 3
	};

void adc_init(void);

uint8_t adc_read(uint8_t channel);




#endif /* ADC_DRIVER_H_ */