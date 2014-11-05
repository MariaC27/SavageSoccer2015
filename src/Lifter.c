#include "Lifter.h"
#include "Controller.h"

const Motor_Port Lifter_motorPort = 6;

const struct {
    Motor_Speed raise;
    Motor_Speed lower;
} Lifter_motorSpeed = {-127, 127};

/**
 * Put code here to initialize the sweeper subsystem.
 */
void Lifter_Init(void) {
}

/**
 * Put code here to initialize the sweeper at the beginning of teleop.
 */
void Lifter_TeleopInit(void) {
}

/**
 * Put code here to update the sweeper based on the controls.
 */
void Lifter_Update(void) {
    if (Controller_IsLifterRaiseButtonPressed()) {
        Lifter_Raise();
    } else if (Controller_IsLifterLowerButtonPressed()) {
        Lifter_Raise();
    } else {
        Lifter_Stop();
    }
}

void Lifter_Raise(void) {
    Motor_set(Lifter_motorPort, Lifter_motorSpeed.raise);
}

void Lifter_Lower(void) {
    Motor_set(Lifter_motorPort, Lifter_motorSpeed.lower);
}

void Lifter_Stop(void) {
    Motor_set(Lifter_motorPort, 0);
}