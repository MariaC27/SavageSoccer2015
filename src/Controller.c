#include "Controller.h"
#include "Util.h"

typedef unsigned char Controller_Port;

/**
 * Internal type representing a channel on the controller.
 */
typedef unsigned char Controller_Channel;

const Controller_Port Controller_port = PORT_1;

const Controller_Channel Controller_driveMoveChannel = CHANNEL_3;
const Controller_Channel Controller_driveRotateChannel = CHANNEL_1;

const Controller_Channel Controller_dumperRaiseButton = CHANNEL_6_TOP;
const Controller_Channel Controller_dumperLowerButton = CHANNEL_6_BOTTOM;

const Controller_Channel Controller_dumperDoorButton = CHANNEL_5_TOP;

const Controller_Channel Controller_driveReverseButton = CHANNEL_5_BOTTOM;

Controller_Position Controller_GetStickAxis(Controller_Port port, Controller_Channel channel) {
    return -ConvertSigned(GetOIAInput(port, channel));
}

bool Controller_IsDumperRaiseButtonPressed(void) {
    return GetOIDInput(Controller_port, Controller_dumperRaiseButton);
}

bool Controller_IsDumperLowerButtonPressed(void) {
    return GetOIDInput(Controller_port, Controller_dumperLowerButton);
}

bool Controller_IsDumperDoorButtonPressed(void) {
    return GetOIDInput(Controller_port, Controller_dumperDoorButton);
}

bool Controller_IsDriveReverseButtonPressed(void) {
    return GetOIDInput(Controller_port, Controller_driveReverseButton);
}

Controller_Position Controller_GetDriveMove(void) {
    return Controller_GetStickAxis(Controller_port, Controller_driveMoveChannel);
}

Controller_Position Controller_GetDriveRotate(void) {
    return Controller_GetStickAxis(Controller_port, Controller_driveRotateChannel);
}