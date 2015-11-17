/* 
 * File:   PID.h
 * Author: Ben Wolsieffer
 *
 * Created on November 5, 2014, 10:30 AM
 *
 * "Object oriented" PID controller. All functions take a pointer to a PID
 * struct, which stores state and tuning settings.
 */

#ifndef PID_H
#define	PID_H

#include "Common.h"

typedef struct {
	// Public

	// PID tuning variables
	int p;
	int i;
	int d;
	/**
	 * Divisor used to scale correctly scale the output.
	 */
	int div;

	// Other settings
	/**
	 * The error magnitude below which the controller is considered to be on
	 * target.
	 */
	int tolerance;
	/**
	 * Maximum magnitude of the integral component, before the tuning parameter
	 * is applied.
	 */
	int maxIntegral;
	/**
	 * The maximum output of the controller.
	 */
	int minOutput;
	/**
	 * The minimum output of the controller.
	 */
	int maxOutput;
	/**
	 * The interval between subsequent PID calculations, in milliseconds.
	 */
	long interval;

	// Private
	int lastError;
	unsigned long lastTime;
	int integral;
	int lastOutput;
	bool reset;
	bool valid;
} PID;

/**
 * Initialized a PID structure with valid default values. Should be called
 * before any of the other PID functions and before tuning variables are set.
 */
void PID_Init(PID* pid);

/**
 * Calculates and returns an output based on the specified input and setpoint.
 * The calculation only occurs after the interval has passed since the last
 * calculation.
 */
int PID_Calc(PID* pid, int input, int setpoint);

/**
 * Resets all the state variables of the PID controller to their default
 * values. Tuning constants and other settings are not modified.
 */
void PID_Reset(PID* pid);

/**
 * Gets whether the PID controller is considered to be on target.
 *
 * @return true if abs(error) < pid->tolerance
 */
bool PID_OnTarget(PID* pid);

#endif	/* PID_H */

