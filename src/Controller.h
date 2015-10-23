/* 
 * File:   Controller.h
 * Author: Ben Wolsieffer
 *
 * Created on November 11, 2013, 10:28 AM
 */

#ifndef CONTROLLER_H
#define	CONTROLLER_H

#include "Common.h"

typedef signed char Controller_Position;

/**
 * Gets the state of the lifter raise button.
 * @return true if the button is pressed
 */
bool Controller_IsLifterRaiseButtonPressed(void);

/**
 * Gets the state of the lifter lower button.
 * @return true if the button is pressed
 */
bool Controller_IsLifterLowerButtonPressed(void);

/**
 * Gets the state of the sweeper in button.
 * @return true if the button is pressed
 */
bool Controller_IsSweeperToggleButtonPressed(void);

/**
 * Gets the state of the sweeper out button.
 * @return true if the button is pressed
 */
bool Controller_IsSweeperOutButtonPressed(void);

/**
 * Gets the value of the joystick that controls the forward/backward movement of
 * the robot.
 * @return the position of the joystick between -127 and 127
 */
Controller_Position Controller_GetDriveMove(void);

/**
 * Gets the value of the joystick that controls the rotation of
 * the robot.
 * @return the position of the joystick between -127 and 127
 */
Controller_Position Controller_GetDriveRotate(void);

#endif	/* CONTROLLER_H */
