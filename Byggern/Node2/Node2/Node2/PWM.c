/*
* PWM.c
*
* Created: 22.10.2020 10:18:06
*  Author: jarlenes
*/


#include <sam.h>
#include <stdio.h>
#include <stdlib.h>

void attach_servo()
{
	PWM ->PWM_ENA |= PWM_ENA_CHID5; // Enable PMW at channel 5
}

void detach_servo()
{
	PWM ->PWM_DIS |= PWM_DIS_CHID5; // Disable PWN at channel 5
}

void pwm_init()
{
	PMC -> PMC_PCER1 |= PMC_PCER1_PID36; //For PMC Peripheral Clock enabler Register 1: We use ID-36
	PIOC -> PIO_PDR |= PIO_PDR_P19; // Disable PIO controller on pin 44
	PIOC -> PIO_ABSR |= PIO_ABSR_P19; // Enable Peripheral B on pin 44
	
	// Peripheral B - Signal: PWMMH5 - I/O-Line: PC19
	PWM -> PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_1024; // Master clock / 1024. Master clock = 84M
	
	PWM -> PWM_CH_NUM[5].PWM_CPRD = 1641;//Channel Period Register, (84M * 20millisek / 1024)
	PWM -> PWM_CH_NUM[5].PWM_CDTY = 1641-123;//123; //Channel duty cycle Register - set the value to 1.5millisec=> 123. 1.5ms equals the servo in middle position
	
}
void pwm_send(float angle) // Sends the desired angle to the servo
{
	
	if(angle > 0.0008 && angle < 0.0022) // The servo from left position(0.0008) to right position (0.0022). Middle position (0.0015)
	{
		PWM -> PWM_CH_NUM[5].PWM_CDTYUPD = 1641-(((84000000*angle)/1024)); //Updates the duty cycle register - Set motor to the desired angle
	}
}
float map(uint8_t x, uint8_t in_min, uint8_t in_max, float out_min, float out_max) // Scale a "x" input from one range to another range. Example x = 20, range: 0-100, change range to 0-5, x = 1
{
	return ((x - in_min) * (out_max - out_min) / (in_max - in_min)) + out_min; // Return the "x" value
}
void servoMap(uint8_t number) // Maps the value for the servo
{
	float angle = map(number, 0, 100, 0.0021, 0.0009); // Returns a value between 0.0009 and 0.0021
	pwm_send(angle); // Sends the value to the servo
}
