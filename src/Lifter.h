/* 
 * File:   Lifter.h
 * Author: Ben Wolsieffer
 *
 * Created on November 7, 2015, 8:40 PM
 */

#ifndef LIFTER_H
#define	LIFTER_H

#include "Common.h"
#include "Motor.h"
#include "PID.h"

typedef struct {
	int p;
	int i;
	int d;
} Lifter_PIDConstants;

extern PID Lifter_velPID;
extern Lifter_PIDConstants Lifter_velPIDDownConstants;
extern Lifter_PIDConstants Lifter_velPIDUpConstants;
extern int Lifter_velInputDiv;

#ifdef SMART_DASHBOARD
extern const rom char* Lifter_velPDownKey;
extern const rom char* Lifter_velIDownKey;
extern const rom char* Lifter_velDDownKey;
extern const rom char* Lifter_velPUpKey;
extern const rom char* Lifter_velIUpKey;
extern const rom char* Lifter_velDUpKey;
extern const rom char* Lifter_velDivKey;
extern const rom char* Lifter_velInputDivKey;
#endif // SMART_DASHBOARD

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

void Lifter_SetTrayDumperExtended(bool extended);

void Lifter_SetLiftSpeed(Motor_Speed speed);

unsigned int Lifter_GetPosition(void);

/**
 * Gets the speed of the lifter in position per 50 ms. This must be called at
 * least once every 50ms for it to return valid values.
 */
int Lifter_GetSpeed(void);

void Lifter_SetTrayAngle(unsigned char angle);

#endif	/* LIFTER_H */

