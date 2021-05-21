/*
 * PWM.h
 *
 * Created: 22.10.2020 10:18:30
 *  Author: jarlenes
 */ 


#ifndef PWM_H_
#define PWM_H_
// Functions
void attach_servo();
void detach_servo();
void pwm_init();
void pwm_send(float angle);
float map(uint8_t x, uint8_t in_min, uint8_t in_max, float out_min, float out_max);
void servoMap(uint8_t number);
#endif /* PWM_H_ */