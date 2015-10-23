#include "Drive.h"
#include "Controller.h"
#include "Util.h"
#include "PID.h"

void Drive_ArcadeImpl(Motor_Speed moveSpeed, Motor_Speed rotateSpeed);
void Drive_SetWheel(Drive_Wheel wheel, Motor_Speed speed);
int Drive_GetGyroAngle(void);

const Sensor_Port Drive_gyroPort = 1;
PID Drive_gyroPID;
const unsigned int Drive_gyroSensitivity = 70;
const unsigned char Drive_gyroDeadband = 3;
const unsigned int Drive_gyroTolerance = 50;
const unsigned int Drive_gyroMaxIntegral = 20;
const float Drive_gyroOutputSpeedScale = 0.05;
const float Drive_gyroP = 0.05;
const float Drive_gyroI = 0.04;
const float Drive_gyroD = 0.0;

const Motor_Speed arcadeRotationDeadband = 10;

const Drive_Wheels Drive_wheels = { { 1, false }, //  Front left {port, inverted}
		{ 2, true }, // Front right
		{ 3, false }, //  Rear left
		{ 4, true } //  Rear right
};

/**
 * Put code here to initialize the drive subsystem. 
 */
void Drive_Init(void) {
	SetGyroType(Drive_gyroPort, Drive_gyroSensitivity);
	SetGyroDeadband(Drive_gyroPort, Drive_gyroDeadband);
	InitGyro(Drive_gyroPort);

	PID_Init(&Drive_gyroPID);
	Drive_gyroPID.p = Drive_gyroP;
	Drive_gyroPID.i = Drive_gyroI;
	Drive_gyroPID.d = Drive_gyroD;
	Drive_gyroPID.tolerance = Drive_gyroTolerance;
	Drive_gyroPID.maxIntegral = Drive_gyroMaxIntegral;
}

/**
 * Put code here to initialize the drive subsystem at the beginning of
 * autonomous. 
 */
void Drive_AutonomousInit(void) {
	StartGyro(Drive_gyroPort);
}

/**
 * Put code here to initialize the drive subsystem at the beginning of teleop. 
 */
void Drive_TeleopInit(void) {
	// Make sure the gyro is started even if autonomous did not run.
	StartGyro(Drive_gyroPort);
}

/**
 * Put code here that will be run in a loop during teleop and drive the robot.
 */
void Drive_Teleop(void) {
	Drive_Arcade(Controller_GetDriveMove(), Controller_GetDriveRotate());
}

void Drive_Arcade(Motor_Speed moveSpeed, Motor_Speed rotateSpeed) {
//    static bool orientationLocked = false;
//    static int orientation = 0;

//    if (abs(rotateSpeed) < arcadeRotationDeadband) {
//        if (!orientationLocked) {
//            PID_Reset(&Drive_gyroPID);
//            orientation = Drive_GetGyroAngle();
//            orientationLocked = true;
//        }
//        Drive_Orientation(moveSpeed, 127, orientation);
//    } else {
//        // Make sure the direction is unlocked
//        orientationLocked = false;
	Drive_ArcadeImpl(moveSpeed, rotateSpeed);
//    }
}

void Drive_ArcadeImpl(Motor_Speed moveSpeed, Motor_Speed rotateSpeed) {
	Motor_Speed leftMotorSpeed;
	Motor_Speed rightMotorSpeed;

	// Do the calculations for arcade drive.
	if (moveSpeed > 0) {
		if (rotateSpeed > 0) {
			leftMotorSpeed = moveSpeed - rotateSpeed;
			rightMotorSpeed = max(moveSpeed, rotateSpeed);
		} else {
			leftMotorSpeed = max(moveSpeed, -rotateSpeed);
			rightMotorSpeed = moveSpeed + rotateSpeed;
		}
	} else {
		if (rotateSpeed > 0) {
			leftMotorSpeed = -max(-moveSpeed, rotateSpeed);
			rightMotorSpeed = moveSpeed + rotateSpeed;
		} else {
			leftMotorSpeed = moveSpeed - rotateSpeed;
			rightMotorSpeed = -max(-moveSpeed, -rotateSpeed);
		}
	}

	// Drive the left and right sides of the robot at the specified speeds.
	Drive_Tank(leftMotorSpeed, rightMotorSpeed);

}

void Drive_Tank(Motor_Speed leftSpeed, Motor_Speed rightSpeed) {
	Drive_SetWheel(Drive_wheels.frontLeft, leftSpeed);
	Drive_SetWheel(Drive_wheels.frontRight, rightSpeed);
	Drive_SetWheel(Drive_wheels.rearLeft, leftSpeed);
	Drive_SetWheel(Drive_wheels.rearRight, rightSpeed);
}

void Drive_Orientation(Motor_Speed speed, Motor_Speed maxRotationSpeed,
		int targetAngle) {
	int rotationSpeed;
	rotationSpeed = PID_Calc(&Drive_gyroPID, Drive_GetGyroAngle(), targetAngle)
			+ (Drive_gyroOutputSpeedScale * abs(speed));
	if (abs(rotationSpeed) > maxRotationSpeed) {
		rotationSpeed = (
				(rotationSpeed < 0) ? (-maxRotationSpeed) : (maxRotationSpeed));
	}

	Drive_ArcadeImpl(speed, rotationSpeed);
}

void Drive_Straight(Motor_Speed speed) {
	Drive_Tank(speed, speed);
}

void Drive_StraightTime(Motor_Speed speed, long rampTime, long time) {
	int angle = Drive_GetGyroAngle();
	const long startTime = GetMsClock();
	const long endTime = startTime + time;
	long currTime;

	while ((currTime = GetMsClock()) < endTime) {
		Motor_Speed realSpeed = speed;
		long elapsedTime = currTime - startTime;
		if (elapsedTime < rampTime) {
			realSpeed = speed * ((float) elapsedTime / rampTime);
		}
		Drive_Orientation(realSpeed, 127, angle);
	}
	Drive_Stop();
}

void Drive_RotateTo(int orientation, Motor_Speed maxRotationSpeed) {
	PID_Reset(&Drive_gyroPID);
	while (!PID_OnTarget(&Drive_gyroPID)) {
		Drive_Orientation(0, maxRotationSpeed, orientation);
	}
	Drive_Stop();
}

void Drive_Rotate(int degrees, Motor_Speed maxRotationSpeed) {
	Drive_RotateTo(Drive_GetGyroAngle() + degrees, maxRotationSpeed);
}

void Drive_Stop(void) {
	Drive_Straight(0);
}

void Drive_SetWheel(Drive_Wheel wheel, Motor_Speed speed) {
	Motor_set(wheel.port, wheel.inverted ? (-speed) : speed);
}

int Drive_GetGyroAngle(void) {
	return -GetGyroAngle(Drive_gyroPort);
}
