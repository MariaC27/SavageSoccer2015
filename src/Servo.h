/* 
 * File:   Servo.h
 * Author: Ben Wolsieffer
 *
 * Created on October 31, 2014, 9:28 AM
 */

#ifndef SERVO_H
#define	SERVO_H

#include "Common.h"

/**
 * A type representing the port of a servo.
 */
typedef unsigned char Servo_Port;

/**
 * A type representing the position of a servo.
 */
typedef unsigned char Servo_Position;

/**
 * Sets the position of a servo.
 * 
 * @param port the port of the servo
 * @param position the position of the servo (-127 to 127)
 */
void Servo_set(Servo_Port port, Servo_Position position);
//Servo_Position Server_get(Servo_Port port);

#endif	/* SERVO_H */

