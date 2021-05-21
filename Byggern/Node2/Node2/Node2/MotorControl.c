/*
* MotorControl.c
*
* Created: 05.11.2020 08:23:56
*  Author: jarlenes
*/
#include <sam.h>
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
#include "Timer.h"
#include "IR.h"

// Global variables
uint16_t startPointEncoder;
uint16_t endPointEncoder;
uint16_t totalDistanceEncoder;
double integral;
double lastError;
uint8_t calibrated = 0;
uint8_t posX =0;
uint8_t posY =0;
uint8_t leftSlider =0;
uint8_t rightButton =0;
uint8_t start =0;
uint8_t leftButton =0;
uint8_t activeS = 0;


void InitMotorControl()
{
	// DAC controller
	PMC -> PMC_PCER1 |= PMC_PCER1_PID38; //For PMC Peripheral Clock enabler Register 1: We use ID-38 for DACC controller
	PMC -> PMC_PCER0 |= PMC_PCER0_PID14; // Enable PIOD
	PMC -> PMC_PCER0 |= PMC_PCER0_PID13; // Enable PIOC
	DACC -> DACC_CHER |= DACC_CHER_CH1; // Enable channel 1
	DACC -> DACC_MR |= DACC_MR_USER_SEL_CHANNEL1; // Choose channel 1
	
	//PINS MJ1, PIO Enable Register
	PIOD -> PIO_PER |= PIO_PER_P10; // DIR: Sets the direction
	PIOD -> PIO_PER |= PIO_PER_P9; // EN: Enables motor
	PIOD -> PIO_PER |= PIO_PER_P2; // Select: Select Hi/Lo byte of encoder
	PIOD -> PIO_PER |= PIO_PER_P1;; // !RST: Active low reset of encoder
	PIOD -> PIO_PER |= PIO_PER_P0; // !OE: Active low output enable of encoder
	
	// PINS MJ1, PIO Output Enable Register
	PIOD ->PIO_OER |= PIO_OER_P10; // Set DIR as output
	PIOD ->PIO_OER |= PIO_OER_P9; // Set EN as output
	PIOD ->PIO_OER |= PIO_OER_P2; // Set Select as output
	PIOD ->PIO_OER |= PIO_OER_P1; // Set !RST as output
	PIOD ->PIO_OER |= PIO_OER_P0; // Set !OE as output
	
	//PINS MJ2, PIO Enable Register
	PIOC -> PIO_PER |= PIO_PER_P1; // DO0
	PIOC -> PIO_PER |= PIO_PER_P2; // DO1
	PIOC -> PIO_PER |= PIO_PER_P3; // DO2
	PIOC -> PIO_PER |= PIO_PER_P4; // DO3
	PIOC -> PIO_PER |= PIO_PER_P5; // DO4
	PIOC -> PIO_PER |= PIO_PER_P6; // DO5
	PIOC -> PIO_PER |= PIO_PER_P7; // DO6
	PIOC -> PIO_PER |= PIO_PER_P8; // DO7
	
	//PINS MJ2, PIO  Output Disable Register
	PIOC ->PIO_ODR |= PIO_ODR_P1; // Set DO0 as input
	PIOC ->PIO_ODR |= PIO_ODR_P2; // Set DO1 as input
	PIOC ->PIO_ODR |= PIO_ODR_P3; // Set DO2 as input
	PIOC ->PIO_ODR |= PIO_ODR_P4; // Set DO3 as input
	PIOC ->PIO_ODR |= PIO_ODR_P5; // Set DO4 as input
	PIOC ->PIO_ODR |= PIO_ODR_P6; // Set DO5 as input
	PIOC ->PIO_ODR |= PIO_ODR_P7; // Set DO6 as input
	PIOC ->PIO_ODR |= PIO_ODR_P8; // Set DO7 as input
	
}
void deactiveSolenoid()
{
	// PIO Controller Set Output Data Register
	PIOC->PIO_SODR = PIO_SODR_P12; // Sets the data to be driven on the I/O line
}
void initSolenoid()
{
	PIOC -> PIO_PER |= PIO_PER_P12; // Enable register
	PIOC -> PIO_OER |= PIO_OER_P12; // Set as output
	deactiveSolenoid();
}
void activeSolenoid()
{
	// PIO Controller Clear Output Data Register
	PIOC->PIO_CODR = PIO_CODR_P12; // Clears the data to be driven on the I/O line
}

void SetMotorSpeed(uint32_t speed)
{
	DACC -> DACC_CDR = DACC_CDR_DATA(speed); // Conversion Data Register, 32 bit
}
void EnableMotor(uint8_t n)
{
	if( n == 1) // Check if the input is 1, then turn on
	{
		PIOD->PIO_SODR = PIO_SODR_P9; // Sets the data to be driven on the I/O line (HIGH)
	}
	else
	{
		PIOD->PIO_CODR = PIO_CODR_P9; // Clears the data to be driven on the I/O line (LOW)
	}
}
void ResetEncoder(uint8_t n)
{
	if( n == 1) // Check if the input is 1, then turn on
	{
		PIOD -> PIO_SODR = PIO_SODR_P1; // Sets the data to be driven on the I/O line (HIGH)
	}
	else
	{
		PIOD -> PIO_CODR = PIO_CODR_P1; // Clears the data to be driven on the I/O line (LOW)
	}
}
void DirectionMotor(uint8_t n)
{
	if( n == 1) // Check if the input is 1, then turn on
	{
		PIOD -> PIO_SODR = PIO_SODR_P10; // Sets the data to be driven on the I/O line (HIGH)
	}
	else
	{
		PIOD->PIO_CODR = PIO_CODR_P10; // Clears the data to be driven on the I/O line (LOW)
	}
}
void SelectEncoderByte(uint8_t n)
{
	if( n == 1) // Check if the input is 1, then turn on
	{
		PIOD -> PIO_SODR = PIO_SODR_P2; // Sets the data to be driven on the I/O line (HIGH)
	}
	else
	{
		PIOD -> PIO_CODR = PIO_CODR_P2; // Clears the data to be driven on the I/O line (LOW)
	}
}
void OutputEnableEncoder(uint8_t n)
{
	if( n == 1) // Check if the input is 1, then turn on
	{
		PIOD -> PIO_SODR = PIO_SODR_P0; // Sets the data to be driven on the I/O line (HIGH)
	}
	else
	{
		PIOD ->PIO_CODR = PIO_CODR_P0; // Clears the data to be driven on the I/O line (LOW)
	}
}


uint8_t readPin(uint8_t pinNumber){ // Reads the pin on the encoder
	// PIO Controller Pin Data Status Register
	return (PIOC->PIO_PDSR & (1<<pinNumber)); // Left shift the pinNumber with 1
}


uint16_t ReadEncoder()
{
	OutputEnableEncoder(0); // Enable output of encoder
	SelectEncoderByte(0); // Set select low for getting high byte
	delay_us(25); // Delay 25 micro sec
	//read MSB
	uint8_t highBytes = 0;
	for (uint8_t i = 1; i<9;i++)  // 8 loops
	{
		highBytes  |= readPin(i); // Reads all the high bits DO8-D15
	}
	
	
	SelectEncoderByte(1); // Set select high for getting low byte
	delay_us(25); // Delay 25 micro sec
	//read LSB
	uint8_t lowBytes = 0;
	for (uint8_t i = 1; i<9;i++)
	{
		lowBytes  |= readPin(i); // Reads all the low bits DO0-DO7
	}
	
	OutputEnableEncoder(1); // Disable output of encoder
	
	return ((highBytes<<8) | lowBytes); // Return the position value of the encoder
}

void calibrationEncoder() // Calibrate for finding the range the encoder is in-between
{
	EnableMotor(1); // Turn on the motor
	DirectionMotor(1); // Direction right
	SetMotorSpeed(2000);
	delay_us(200); // Micro sec
	SetMotorSpeed(1000);
	delay_ms(1300); // Milli sec
	ResetEncoder(0); // Clears the data
	delay_us(20);
	ResetEncoder(1); // Sets the data
	startPointEncoder = ReadEncoder();
	DirectionMotor(0); // Direction left
	SetMotorSpeed(600);
	delay_ms(1500);
	endPointEncoder = ReadEncoder(); // Endpoint
	totalDistanceEncoder = endPointEncoder-startPointEncoder; // The range the encoder moves in-between
	calibrated = 1; // Finish with calibration
	
}


uint16_t map1(uint8_t x, uint8_t in_min, uint8_t in_max, uint16_t out_min, uint16_t out_max) // Scale a "x" input from one range to another range. Example x = 20, range: 0-100, change range to 0-5, x = 1
{
	return ((x - in_min) * (out_max - out_min) / (in_max - in_min)) + out_min; // Return the "x" value
}





double constrain(int value, int min, int max){ // Limit the value to be in-between two numbers
	if (value>max)
	{
		value = max;
	}
	else if (value<min)
	{
		value = min;
	}
	
	return (double)value;
	
}

void PIDcontroller(double targetPos,double kp, double ki, double kd)
{
	double current_position = ReadEncoder();
	if(current_position > 25000) // If error in the system and the current position go out of the range, we set it to 0
	{
		ResetEncoder(0);
		delay_us(10);
		ResetEncoder(1);
		current_position=0;
	}
	double error = targetPos - current_position; // The length between the desired position and the actual position
	integral = integral + error; // Adds up the error
	double derivative = error - lastError; // Check the error against the last error
	double output = abs((error*kp) +(integral*ki) + (derivative*kd)); // Calculate the speed based on how close it to the desired position
	
	output = constrain((int)(output),0,1300); // Limits the value to be in-between 0-1300
	if (error > 0) // To much to right
	{
		DirectionMotor(0); // Direction left
		SetMotorSpeed((int)output);
	}
	else{ // To much to left
		DirectionMotor(1); // Direction right
		SetMotorSpeed((int)output);
	}
	
	lastError = error;
}

void resetPID(){
	integral =0;
	lastError=0;
}

void sliderMap(uint8_t value){
	uint16_t position = map1(value,0,255,endPointEncoder-1000,1000); // Offset on 1000 for the corners
	PIDcontroller(position,0.1,0.001,0.15); // Sends the position, Kp, Ki and Kd

	void TC0_Handler()
	{
		if (start & !calibrated) // If new game starts and it is not calibrated
		{
			calibrationEncoder(); // Calibrate
			attach_servo(); // Turn on servo
			calibrated = 1; // Calibration finished
		}
		if (rightButton) // Stops the game
		{
			pwm_send(0.0015); // Set the servo to mid position
			delay_ms(10);
			SetMotorSpeed(0); // Turn of the motor speed
			delay_ms(10);
			EnableMotor(0); // Turn of the motor
			delay_ms(10);
			detach_servo(); // Turn of the servo
			sendScore();
			resetScore(); // Set the score to 0
			delay_ms(50);
			sendScore(); // Sends the score when is 0
			resetPID();
			calibrated = 0; // Calibration false
			disableTimer(); // Turn off the timer
			
		}
		if (leftButton) // Shoot with the solenoid
		{
			activeSolenoid();
			activeS = 1;
			
		}
		else if (activeS) // Must been activated before deactived it
		{
			deactiveSolenoid(); // Return to normal position
			activeS = 0;
		}
		

		sliderMap(leftSlider); // Sends the desired encoder position
		servoMap(posX); // Sends the desired servo position

		goalScored();
		uint32_t status;
		status = TC0->TC_CHANNEL[0].TC_SR; // Reads the status register
		NVIC_ClearPendingIRQ((IRQn_Type)ID_TC0); // Clear IQRn pending status
	}