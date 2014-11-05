/* 
 * File:   Drive.h
 * Author: Ben Wolsieffer
 *
 * Created on November 14, 2013, 2:29 PM
 */

#ifndef DRIVE_H
#define	DRIVE_H

#include "Common.h"
#include "Motor.h"

typedef struct {
    const Motor_Port port;
    const bool inverted;
} Drive_Wheel;

typedef struct {
    const Drive_Wheel frontLeft;
    const Drive_Wheel frontRight;
    const Drive_Wheel rearLeft;
    const Drive_Wheel rearRight;
} Drive_Wheels;
const extern Drive_Wheels Drive_wheels;

/**
 * Runs once when the robot is powered on.
 */
void Drive_Init(void);

/**
 * Runs once at the beginning of teleop.
 */
void Drive_TeleopInit(void);

/**
 * Teleop drive loop. Called repeatedly during teleop.
 */
void Drive_Teleop(void);

/**
 * Drives the robot using the specified forward and rotation speeds.
 * 
 * @param moveSpeed the speed to move forward at (-127 to 127)
 * @param rotateSpeed the speed to rotate at (-127 to 127)
 */
void Drive_Arcade(Motor_Speed moveSpeed, Motor_Speed rotateSpeed);

/**
 * Drives the left and right sides of the robot at the specified speeds.
 *
 * @param leftSpeed the speed of the left side of the robot (-127 to 127)
 * @param rightSpeed the speed of the right side of the robot (-127 to 127)
 */
void Drive_Tank(Motor_Speed leftSpeed, Motor_Speed rightSpeed);

/**
 * Drives the robot in a straight line at the specified speed.
 * 
 * @param speed the speed to drive at (-127 to 127)
 */
void Drive_Straight(Motor_Speed speed);

/**
 * Stops the robot.
 */
void Drive_Stop(void);

#endif	/* DRIVE_H */

