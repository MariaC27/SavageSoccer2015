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
    float p;
    float i;
    float d;
    int tolerance;
    int minOutput;
    int maxOutput;
    // Private
    int lastError;
    unsigned long lastTime;
    float integral;
    int lastOutput;
    bool reset;
} PID;

void PID_Init(PID* pid);

int PID_Calc(PID* pid, int input, int setpoint);

void PID_Reset(PID* pid);

bool PID_OnTarget(PID* pid);

#endif	/* PID_H */

