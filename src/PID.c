#include "PID.h"

void PID_Init(PID* pid) {
    pid->p = 0;
    pid->i = 0;
    pid->d = 0;
    pid->setpoint = 0;
    pid->input = 0;

    pid->integral = 0;
    pid->lastError = 0;
    pid->lastTime = 0;
}

int PID_Calc(PID* pid) {
    unsigned long currTime = GetMsClock();
    unsigned long elapsedTime;
    int error;
    float pTerm;
    float iTerm;
    float dTerm;

    if (pid->lastTime == 0) {
        pid->lastTime = currTime;
    }

    elapsedTime = currTime - pid->lastTime;
    error = pid->setpoint - pid->input;

    // Do the PID calculation
    pTerm = pid->p * error;
    pid->integral += (float) error / elapsedTime;
    iTerm = pid->i * pid->integral;
    dTerm = pid->d * ((float) (error - pid->lastError) / elapsedTime);

    return (int) (pTerm + iTerm + dTerm);
    return 0;
}