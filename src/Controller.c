#include "Controller.h"
#include "Util.h"

typedef unsigned char Controller_Port;

/**
 * Internal type representing a channel on the controller.
 */
typedef unsigned char Controller_Channel;

const Controller_Port Controller_port = PORT_1;

const Controller_Channel Controller_sweeperToggleButton = CHANNEL_5_TOP;
const Controller_Channel Controller_sweeperOutButton = CHANNEL_5_BOTTOM;

const Controller_Channel Controller_lifterRaiseButton = CHANNEL_6_TOP;
const Controller_Channel Controller_lifterLowerButton = CHANNEL_6_BOTTOM;

const Controller_Channel Controller_driveMoveChannel = CHANNEL_3;
const Controller_Channel Controller_driveRotateChannel = CHANNEL_1;

Controller_Position Controller_GetStickAxis(Controller_Port port,
		Controller_Channel channel) {
	// For some reason, up and left are negative on the controller, so invert
	// the axis.
	return -ConvertSigned(GetOIAInput(port, channel));
}

bool Controller_IsLifterRaiseButtonPressed(void) {
	return GetOIDInput(Controller_port, Controller_lifterRaiseButton);
}

bool Controller_IsLifterLowerButtonPressed(void) {
	return GetOIDInput(Controller_port, Controller_lifterLowerButton);
}

bool Controller_IsSweeperToggleButtonPressed(void) {
	return GetOIDInput(Controller_port, Controller_sweeperToggleButton);
}

bool Controller_IsSweeperOutButtonPressed(void) {
	return GetOIDInput(Controller_port, Controller_sweeperOutButton);
}

Controller_Position Controller_GetDriveMove(void) {
	return Controller_GetStickAxis(Controller_port, Controller_driveMoveChannel);
}

Controller_Position Controller_GetDriveRotate(void) {
	return Controller_GetStickAxis(Controller_port,
			Controller_driveRotateChannel);
}
