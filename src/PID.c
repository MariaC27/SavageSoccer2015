#include "PID.h"
#include "Util.h"

void PID_Init(PID* pid) {
    // Give all the 
    pid->p = 0;
    pid->i = 0;
    pid->d = 0;
    pid->tolerance = 0;
    pid->maxIntegral = 100;
    pid->minOutput = -127;
    pid->maxOutput = 127;

    PID_Reset(pid);
}

int PID_Calc(PID* pid, int input, int setpoint) {
    // All variables must be declared at the start of their scope... arrgh
    unsigned long currTime = GetUsClock();
    unsigned long elapsedTime;
    int error;
    float pTerm;
    float iTerm;
    float dTerm;
    int output;

    // If controller was reset (or not initialized), set lastTime to the current
    // time and skip this iteration of the loop.
    if (pid->reset) {
        pid->lastTime = currTime;
        pid->reset = false;
    }

    elapsedTime = currTime - pid->lastTime;

    // Only update the output if more than one microsecond has passed to avoid
    // divide by zero errors.
    if (elapsedTime >= 1) {
        // error = difference between setpoint and the actual value
        error = setpoint - input;

        // proportional term = P gain * error
        pTerm = pid->p * error;

        // Calculate the new integral
        pid->integral += error * elapsedTime;

        // Prevent integral windup
        if (abs(pid->integral) > pid->maxIntegral) {
            pid->integral = ((pid->integral < 0) ? (-pid->maxIntegral) : (pid->maxIntegral));
        }

        // integral term = I gain * integral
        iTerm = pid->i * pid->integral;

        // derivative term = D gain * rate of change of error
        dTerm = pid->d * ((float) (error - pid->lastError) / elapsedTime);

        // Sum the terms to get the (unbounded) output.
        output = (int) (pTerm + iTerm + dTerm);

        // Limit the output to the possible values for the system
        if (output > pid->maxOutput) {
            output = pid->maxOutput;
        } else if (output < pid->minOutput) {
            output = pid->minOutput;
        }

        pid->lastError = error;
        pid->valid = true;
        // Store the output for the next iteration and return it
        return pid->lastOutput = output;
    } else {
        return pid->lastOutput;
    }
}

void PID_Reset(PID* pid) {
    // Reset the integral
    pid->integral = 0;
    // Reset the previous error value
    pid->lastError = 0;
    // Reset the last output value
    pid->lastOutput = 0;
    // Set the reset flag to tell the controller to reinitialize these variables
    pid->reset = true;
    pid->valid = false;
}

bool PID_OnTarget(PID* pid) {
    return (abs(pid->lastError) < pid->tolerance) && (pid->valid);
}