/* 
 * File:   Sweeper.h
 * Author: Ben Wolsieffer
 *
 * Created on November 4, 2014, 4:58 PM
 */

#ifndef SWEEPER_H
#define	SWEEPER_H

#include "Common.h"
#include "PID.h"
#include "Motor.h"

//extern PID Sweeper_PID;

//extern const rom char* Sweeper_pKey;
//extern const rom char* Sweeper_iKey;
//extern const rom char* Sweeper_dKey;
//extern const rom char* Sweeper_divKey;

/**
 * Runs once when the robot is powered on.
 */
void Sweeper_Init(void);

/**
 * Runs once at the beginning of teleop.
 */
void Sweeper_TeleopInit(void);

/**
 * Runs in a loop during teleop.
 */
void Sweeper_Update(void);

void Sweeper_SetSpeed(Motor_Speed speed);

//void Sweeper_SetPosition(int angle, bool positiveDir);

//long Sweeper_GetPosition(void);

/**
 * Starts the sweeper sweeping in the balls.
 */
void Sweeper_SweepIn(void);

/**
 * Starts the sweeper sweeping out the balls.
 */
void Sweeper_SweepOut(void);

/**
 * Stops the sweeper.
 */
void Sweeper_Stop(void);

#endif	/* SWEEPER_H */

