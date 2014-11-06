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
    int setpoint;
    int input;
    // Private
    int lastError;
    unsigned long lastTime;
    float integral;
} PID;

void PID_Init(PID* pid);

int PID_Calc(PID* pid);

#endif	/* PID_H */

