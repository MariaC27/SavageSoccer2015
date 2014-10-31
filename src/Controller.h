/* 
 * File:   Controller.h
 * Author: Ben Wolsieffer
 *
 * Created on November 11, 2013, 10:28 AM
 */

#ifndef CONTROLLER_H
#define	CONTROLLER_H

#include "Common.h"

/**
 * Gets the state of the dumper raise button on the controller.
 * @return true if the button is pressed
 */
bool Controller_IsDumperRaiseButtonPressed(void);

/**
 * Gets the state of the dumper lower button on the controller.
 * @return true if the button is pressed
 */
bool Controller_IsDumperLowerButtonPressed(void);

/**
 * Gets the state of the dumper door button on the controller.
 * @return true if the button is pressed
 */
bool Controller_IsDumperDoorButtonPressed(void);

/**
 * Gets the state of the dumper door button on the controller.
 * @return true if the button is pressed
 */
bool Controller_IsDriveReverseButtonPressed(void);

/**
 * Gets the value of the joystick that controls the forward/backward movement of
 * the robot.
 * @return the position of the joystick between -127 and 127
 */
unsigned char Controller_GetDriveMove(void);

/**
 * Gets the value of the joystick that controls the rotation of
 * the robot.
 * @return the position of the joystick between -127 and 127
 */
unsigned char Controller_GetDriveRotate(void);

#endif	/* CONTROLLER_H */