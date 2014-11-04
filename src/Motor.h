/* 
 * File:   Motor.h
 * Author: Ben Wolsieffer
 *
 * Created on October 30, 2014, 7:58 PM
 */

#ifndef MOTOR_H
#define	MOTOR_H

#include "Common.h"

/**
 * Type representing a motor port.
 */
typedef unsigned char Motor_Port;

/**
 * Type representing the speed of a motor. This value is signed and should be
 * used in all functions that take a motor speed.
 */
typedef signed char Motor_Speed;

/**
 * Sets the speed of a motor.
 * 
 * @param the port of the motor
 * @param speed the speed of the motor (-127 to 127)
 */
void Motor_set(Motor_Port port, Motor_Speed speed);
//Motor_Speed Motor_get(Motor_Port port);

#endif	/* MOTOR_H */

