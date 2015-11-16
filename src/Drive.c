#include "Drive.h"
#include "Controller.h"
#include "Util.h"

void Drive_SetWheel(Drive_Wheel wheel, Motor_Speed speed);

const Drive_Wheels Drive_wheels = { { 1, false }, //  Left {port, inverted}
		{ 2, true } // Right
};

/**
 * Put code here to initialize the drive subsystem. 
 */
void Drive_Init(void) {
}

/**
 * Put code here to initialize the drive subsystem at the beginning of
 * autonomous. 
 */
void Drive_AutonomousInit(void) {
}

/**
 * Put code here to initialize the drive subsystem at the beginning of teleop. 
 */
void Drive_TeleopInit(void) {
}

/**
 * Put code here that will be run in a loop during teleop and drive the robot.
 */
void Drive_Teleop(void) {
	Drive_Arcade(Controller_GetDriveMove(), Controller_GetDriveRotate());
}

void Drive_Arcade(Motor_Speed moveSpeed, Motor_Speed rotateSpeed) {
	Motor_Speed leftMotorSpeed;
	Motor_Speed rightMotorSpeed;

	// Do the calculations for arcade drive.
	if (moveSpeed > 0) {
		if (rotateSpeed > 0) {
			leftMotorSpeed = moveSpeed - rotateSpeed;
			rightMotorSpeed = Max(moveSpeed, rotateSpeed);
		} else {
			leftMotorSpeed = Max(moveSpeed, -rotateSpeed);
			rightMotorSpeed = moveSpeed + rotateSpeed;
		}
	} else {
		if (rotateSpeed > 0) {
			leftMotorSpeed = -Max(-moveSpeed, rotateSpeed);
			rightMotorSpeed = moveSpeed + rotateSpeed;
		} else {
			leftMotorSpeed = moveSpeed - rotateSpeed;
			rightMotorSpeed = -Max(-moveSpeed, -rotateSpeed);
		}
	}

	// Drive the left and right sides of the robot at the specified speeds.
	Drive_Tank(leftMotorSpeed, rightMotorSpeed);
}

void Drive_Tank(Motor_Speed leftSpeed, Motor_Speed rightSpeed) {
	Drive_SetWheel(Drive_wheels.left, leftSpeed);
	Drive_SetWheel(Drive_wheels.right, rightSpeed);
}

void Drive_Straight(Motor_Speed speed) {
	Drive_Tank(speed, speed);
}

void Drive_Stop(void) {
	Drive_Straight(0);
}

void Drive_SetWheel(Drive_Wheel wheel, Motor_Speed speed) {
	Motor_set(wheel.port, wheel.inverted ? (-speed) : speed);
}
