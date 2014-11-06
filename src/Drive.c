#include "Drive.h"
#include "Controller.h"
#include "Util.h"
#include "PID.h"
#include <stdlib.h>

void Drive_SetWheel(Drive_Wheel wheel, Motor_Speed speed);

const Sensor_Port Drive_gyroPort = 1;
PID gyroPID;
const unsigned char Drive_gyroSensitivity = 70;
const unsigned char Drive_gyroDeadband = 3;
const float Drive_gyroP = 0.0;
const float Drive_gyroI = 0.0;
const float Drive_gyroD = 0.0;

const Drive_Wheels Drive_wheels = {
    {1, false}, //  Front left {port, inverted}
    {2, true}, // Front right
    {3, false}, //  Rear left
    {4, true} //  Rear right
};

/**
 * Put code here to initialize the drive subsystem. 
 */
void Drive_Init(void) {
    InitGyro(Drive_gyroPort);
    SetGyroType(Drive_gyroPort, Drive_gyroSensitivity);
    SetGyroDeadband(Drive_gyroPort, Drive_gyroDeadband);
}

/**
 * Put code here to initialize the drive subsystem at the beginning of
 * autonomous. 
 */
void Drive_AutonomousInit(void) {
    StartGyro(Drive_gyroPort);

    PID_Init(&gyroPID);
    gyroPID.p = Drive_gyroP;
    gyroPID.i = Drive_gyroI;
    gyroPID.d = Drive_gyroD;
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

void Drive_Direction(Motor_Speed speed, Motor_Speed maxRotationSpeed, int targetAngle) {
    int rotationSpeed;

    gyroPID.setpoint = targetAngle;
    gyroPID.input = GetGyroAngle(Drive_gyroPort);

    rotationSpeed = PID_Calc(&gyroPID);
    if (abs(rotationSpeed) > maxRotationSpeed) {
        rotationSpeed = maxRotationSpeed;
    }

    Drive_Arcade(speed, 0);
}

void Drive_Straight(Motor_Speed speed) {
    Drive_Tank(speed, speed);
}

void Drive_StraightTime(Motor_Speed speed, long time) {
    int angle = GetGyroAngle(Drive_gyroPort);
    int endTime = GetMsClock() + time;

    while (GetMsClock() < endTime) {
        Drive_Direction(speed, 127, angle);
    }
    Drive_Stop();
}

void Drive_Stop(void) {
    Drive_Straight(0);
}

void Drive_SetWheel(Drive_Wheel wheel, Motor_Speed speed) {
    Motor_set(wheel.port, wheel.inverted ? (-speed) : speed);
}