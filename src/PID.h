/* 
 * File:   PID.h
 * Author: Ben Wolsieffer
 *
 * Created on November 5, 2014, 10:30 AM
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
	int div;

	// Other settings
	int tolerance;
	int maxIntegral;
	int minOutput;
	int maxOutput;
	long interval;

	// Private
	int lastError;
	unsigned long lastTime;
	int integral;
	int lastOutput;
	bool reset;
	bool valid;
} PID;

void PID_Init(PID* pid);

int PID_Calc(PID* pid, int input, int setpoint);

void PID_Reset(PID* pid);

bool PID_OnTarget(PID* pid);

#endif	/* PID_H */

