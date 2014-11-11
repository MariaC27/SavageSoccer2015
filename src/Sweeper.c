#include "Sweeper.h"
#include "Motor.h"
#include "Controller.h"

const Motor_Port Sweeper_motorPort = 5;

const struct {
    Motor_Speed in;
    Motor_Speed out;
} Sweeper_motorSpeed = {127, -127};

/**
 * Put code here to initialize the sweeper subsystem.
 */
void Sweeper_Init(void) {
}

/**
 * Put code here to initialize the sweeper at the beginning of teleop.
 */
void Sweeper_TeleopInit(void) {
}

/**
 * Put code here to update the sweeper based on the controls.
 */
void Sweeper_Update(void) {
    static bool wasSweeperToggleButtonPressed = false;
    static bool sweeperOn = false;

    if (Controller_IsSweeperToggleButtonPressed()) {
        if(!wasSweeperToggleButtonPressed) {
            sweeperOn = !sweeperOn;
            wasSweeperToggleButtonPressed = true;
        }
    } else {
        wasSweeperToggleButtonPressed = false;
    }
    
    if(Controller_IsSweeperOutButtonPressed()) {
        Sweeper_SweepOut();
    } else {
        if(sweeperOn) {
            Sweeper_SweepIn();
        } else {
            Sweeper_Stop();
        }
    }
}

void Sweeper_SweepIn(void) {
    Motor_set(Sweeper_motorPort, Sweeper_motorSpeed.in);
}

void Sweeper_SweepOut(void) {
    Motor_set(Sweeper_motorPort, Sweeper_motorSpeed.out);
}

void Sweeper_Stop(void) {
    Motor_set(Sweeper_motorPort, 0);
}