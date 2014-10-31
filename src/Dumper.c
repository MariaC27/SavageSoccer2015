#include "Dumper.h"
#include "Controller.h"
#include "Util.h"
#include "Motor.h"

void Dumper_OpenEndDoor(void);
void Dumper_CloseEndDoor(void);
void Dumper_OpenMiddleDoor(void);
void Dumper_CloseMiddleDoor(void);
bool Dumper_IsUpperLimitSwitchPressed(void);

typedef struct {
    unsigned char opened;
    unsigned char closed;
} Dumper_DoorRange;

const Motor_Port Dumper_motorPort = 5;
const struct {
    Motor_Speed raise;
    Motor_Speed lower;
} Dumper_motorSpeed = {-127, 127};

const Motor_Port Dumper_endDoorServoPort = 6;
const Dumper_DoorRange Dumper_endDoorServoRange = {186, 136};

const Motor_Port Dumper_middleDoorServoPort = 7;
const Dumper_DoorRange Dumper_middleDoorServoRange = {186, 127};

const Motor_Port Dumper_upperLimitSwitchPort = 1;

Dumper_DoorState doorState;

/**
 * Put code here to initialize the dumper subsystem.
 */
void Dumper_Init(void) {
    Dumper_SetDoorState(MIDDLE_OPEN_END_CLOSED);
}

/**
 * Put code here to initialize the dumper at the beginning of teleop.
 */
void Dumper_TeleopInit(void) {
}

/**
 * Put code here to update the dumper based on the controls.
 */
void Dumper_Update(void) {
    static bool doorButtonPressed = false;
    if (Controller_IsDumperRaiseButtonPressed() && !Dumper_IsUpperLimitSwitchPressed()) {
        Dumper_Raise();
    } else if (Controller_IsDumperLowerButtonPressed()) {
        Dumper_Lower();
    } else {
        Dumper_Stop();
    }

    if (!doorButtonPressed) {
        if (Controller_IsDumperDoorButtonPressed()) {
            doorButtonPressed = true;

            doorState++;
            if (doorState > MIDDLE_OPEN_END_OPEN) {
                doorState = MIDDLE_OPEN_END_CLOSED;
            }

            Dumper_SetDoorState(doorState);
        }
    } else {
        if (!Controller_IsDumperDoorButtonPressed()) {
            doorButtonPressed = false;
        }
    }
}

void Dumper_SetDoorState(Dumper_DoorState ds) {
    doorState = ds;

    switch (doorState) {
        case MIDDLE_OPEN_END_CLOSED:
            Dumper_OpenMiddleDoor();
            Dumper_CloseEndDoor();
            break;
        case MIDDLE_CLOSED_END_CLOSED:
            Dumper_CloseMiddleDoor();
            Dumper_CloseEndDoor();
            break;
        case MIDDLE_CLOSED_END_OPEN:
            Dumper_CloseMiddleDoor();
            Dumper_OpenEndDoor();
            break;
        case MIDDLE_OPEN_END_OPEN:
            Dumper_OpenMiddleDoor();
            Dumper_OpenEndDoor();
    }
}

void Dumper_Raise(void) {
    Motor_set(Dumper_motorPort, Dumper_motorSpeed.raise);
}

void Dumper_Lower(void) {
    Motor_set(Dumper_motorPort, Dumper_motorSpeed.lower);
}

void Dumper_Stop(void) {
    Motor_set(Dumper_motorPort, 0);
}

void Dumper_OpenEndDoor(void) {
    SetPWM(Dumper_endDoorServoPort, Dumper_endDoorServoRange.opened);
}

void Dumper_CloseEndDoor(void) {
    SetPWM(Dumper_endDoorServoPort, Dumper_endDoorServoRange.closed);
}

void Dumper_OpenMiddleDoor(void) {
    SetPWM(Dumper_middleDoorServoPort, Dumper_middleDoorServoRange.opened);
}

void Dumper_CloseMiddleDoor(void) {
    SetPWM(Dumper_middleDoorServoPort, Dumper_middleDoorServoRange.closed);
}

bool Dumper_IsUpperLimitSwitchPressed(void) {
    return !GetDigitalInput(Dumper_upperLimitSwitchPort);
}