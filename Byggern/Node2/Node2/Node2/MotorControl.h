/*
 * MotorControl.h
 *
 * Created: 05.11.2020 08:24:15
 *  Author: jarlenes
 */ 


#ifndef MotorControl_H_
#define MotorControl_H_
// Global variables
uint8_t posX;
uint8_t posY;
uint8_t leftSlider;
uint8_t rightButton;
uint8_t start;
uint8_t leftButton;
uint8_t calibrated;
// Functions
void InitMotorControl();
void initSolenoid();
void activeSolenoid();
void deactiveSolenoid();
void SetMotorSpeed(uint32_t speed);
void EnableMotor(uint8_t n);
void ResetEncoder(uint8_t n);
void DirectionMotor(uint8_t n);
void SelectEncoder(uint8_t n);
void OutputEnable(uint8_t n);
uint16_t ReadEncoder();
uint8_t readPin(uint8_t pinNumber);
void calibrationEncoder();

uint16_t map1(uint8_t x, uint8_t in_min, uint8_t in_max, uint16_t out_min, uint16_t out_max);
void sliderMap(uint8_t value);

double constrain(int value, int min, int max);
void PIDcontroller(double targetPos, double kp, double ki, double kd);
void resetPID();
void TC0_Handler();


#endif /* MotorControl_H_ */