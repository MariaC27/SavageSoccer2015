/* 
 * File:   Lifter.h
 * Author: Ben Wolsieffer
 *
 * Created on November 4, 2014, 7:32 PM
 */

#ifndef LIFTER_H
#define	LIFTER_H

#include "Common.h"
#include "Motor.h"

/**
 * Runs once when the robot is powered on.
 */
void Lifter_Init(void);

/**
 * Runs once at the beginning of teleop.
 */
void Lifter_TeleopInit(void);

/**
 * Runs in a loop during teleop.
 */
void Lifter_Update(void);

/**
 * Starts the lifter raising up the balls.
 */
void Lifter_Raise(void);

/**
 * Starts the lifter lowering the balls.
 */
void Lifter_Lower(void);

/**
 * Stops the lifter.
 */
void Lifter_Stop(void);

#endif	/* LIFTER_H */

