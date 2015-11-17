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

//#define LIFTER_PID

typedef struct {
	int p;
	int i;
	int d;
} Lifter_PIDConstants;

#ifdef LIFTER_PID
extern PID Lifter_PID;
extern Lifter_PIDConstants Lifter_PIDDownConstants;
extern Lifter_PIDConstants Lifter_PIDUpConstants;

#ifdef SMART_DASHBOARD
extern const rom char* Lifter_pDownKey;
extern const rom char* Lifter_iDownKey;
extern const rom char* Lifter_dDownKey;
extern const rom char* Lifter_pUpKey;
extern const rom char* Lifter_iUpKey;
extern const rom char* Lifter_dUpKey;
extern const rom char* Lifter_divKey;
extern const rom char* Lifter_velInputDivKey;
#endif // SMART_DASHBOARD
#endif // LIFTER_PID

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
 * Sets whether the dumper tray is extended.
 *
 * @param true if the tray is extended
 */
void Lifter_SetTrayDumperExtended(bool extended);

/**
 * Sets the speed of the lifter.
 *
 * @param the lifter speed
 */
void Lifter_SetLiftSpeed(Motor_Speed speed);

/**
 * Gets the position of the lifter as measured by the potentiometer. This value
 * can theoretically range between 0 and 1024, but when the lifter is lowered
 * the value is around 960. When the lifter is fully extended (on the floor,
 * outside the robot), this method will return 0 because this is beyond the
 * electrical range of the potentiometer.
 *
 * @return the position of the lifter.
 */
unsigned int Lifter_GetPosition(void);

/**
 * Gets the speed of the lifter in position per 50 ms. This must be called at
 * least once every 50ms for it to return valid values.
 *
 * @return the speed measured by the potentiometer
 */
int Lifter_GetSpeed(void);

/**
 * Sets the angle of the lifter tray. The angle is given as a value between 0
 * and 255, with 127 being flat.
 *
 * @param angle the angle of the tray
 */
void Lifter_SetTrayAngle(unsigned char angle);

#endif	/* LIFTER_H */

