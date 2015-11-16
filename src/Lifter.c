#include "Lifter.h"
#include "Motor.h"
#include "Controller.h"
#include "SmartDashboard.h"
#include "Sweeper.h"
#include "Util.h"

static void Lifter_SetPIDConstants(Lifter_PIDConstants* constants);

const Motor_Port Lifter_leftArmMotorPort = 3;
const Motor_Port Lifter_rightArmMotorPort = 4;
const unsigned char Lifter_leftTrayServoPort = 5;
const unsigned char Lifter_rightTrayServoPort = 6;
const unsigned char Lifter_trayDumperServoPort = 8;

const unsigned char Lifter_potPort = 1;

Ramper Lifter_speedRamper;
Ramper Lifter_positionRamper;

PID Lifter_PID;
Lifter_PIDConstants Lifter_PIDDownConstants;
Lifter_PIDConstants Lifter_PIDUpConstants;

#ifdef SMART_DASHBOARD
const rom char* Lifter_pDownKey = "L_pD";
const rom char* Lifter_iDownKey = "L_iD";
const rom char* Lifter_dDownKey = "L_dD";
const rom char* Lifter_pUpKey = "L_pU";
const rom char* Lifter_iUpKey = "L_iU";
const rom char* Lifter_dUpKey = "L_dU";
const rom char* Lifter_divKey = "L_div";
#endif // SMART_DASHBOARD

const struct {
	unsigned char retract;
	unsigned char extend;
} Lifter_trayDumperPosition = { 180, 10 };

/**
 * Put code here to initialize the lifter subsystem.
 */
void Lifter_Init(void) {
	PID_Init(&Lifter_PID);
	Lifter_PIDDownConstants.p = 0;
	Lifter_PIDDownConstants.i = 0;
	Lifter_PIDDownConstants.d = 0;
	Lifter_PIDUpConstants.p = 0;
	Lifter_PIDUpConstants.i = 0;
	Lifter_PIDUpConstants.d = 0;
	Lifter_PID.div = 16;
	Lifter_PID.maxIntegral = 60;
	Lifter_PID.maxOutput = 80;

	Lifter_SetPIDConstants(&Lifter_PIDUpConstants);

	Lifter_speedRamper.interval = 5;
	Lifter_speedRamper.maxDelta = 1;
	Lifter_speedRamper.type = RAMPER_TYPE_UP;

	Lifter_positionRamper.interval = 5;
	Lifter_positionRamper.maxDelta = 1;
	Lifter_positionRamper.type = RAMPER_TYPE_DOWN | RAMPER_TYPE_UP;

	Lifter_SetTrayDumperExtended(false);

#ifdef SMART_DASHBOARD
	SmartDashboard_putInt(Lifter_pDownKey, Lifter_PIDDownConstants.p);
	SmartDashboard_putInt(Lifter_iDownKey, Lifter_PIDDownConstants.i);
	SmartDashboard_putInt(Lifter_dDownKey, Lifter_PIDDownConstants.d);
	SmartDashboard_putInt(Lifter_pUpKey, Lifter_PIDUpConstants.p);
	SmartDashboard_putInt(Lifter_iUpKey, Lifter_PIDUpConstants.i);
	SmartDashboard_putInt(Lifter_dUpKey, Lifter_PIDUpConstants.d);
	SmartDashboard_putInt(Lifter_divKey, Lifter_PID.div);
#endif // SMART_DASHBOARD
}

/**
 * Put code here to initialize the lifter at the beginning of teleop.
 */
void Lifter_TeleopInit(void) {
}

/**
 * Put code here to update the sweeper based on the controls.
 */
void Lifter_Update(void) {
	unsigned int lifterPos = Lifter_GetPosition();

	// Get the lifter speed from the controller, scale it down, and ramp it
	Controller_Position lifterStickSpeed = Ramper_Ramp(&Lifter_speedRamper,
			Controller_GetLifterSpeed() * 2 / 3);
	Motor_Speed lifterSpeed;

#ifdef LIFTER_PID
	static int setpoint = 9999;
	int error;

	if (setpoint == 9999) {
		setpoint = lifterPos;
		Lifter_positionRamper.lastOutput = setpoint;
	}
	if (lifterStickSpeed < 0) {
		setpoint = 1024;
	} else {
		setpoint = 0;
	}

	Lifter_positionRamper.maxDelta = lifterStickSpeed / 5;

	setpoint = Ramper_Ramp(&Lifter_positionRamper, setpoint);

	error = setpoint - lifterPos;

	if (lifterPos > 420) {
		if (error < 0) {
			Lifter_SetPIDConstants(&Lifter_PIDDownConstants);
		} else {
			Lifter_SetPIDConstants(&Lifter_PIDUpConstants);
		}
	} else {
		if (error > 0) {
			Lifter_SetPIDConstants(&Lifter_PIDDownConstants);
		} else {
			Lifter_SetPIDConstants(&Lifter_PIDUpConstants);
		}
	}

	lifterSpeed = PID_Calc(&Lifter_PID, lifterPos, setpoint);
#else
	lifterSpeed = lifterStickSpeed;
#endif // LIFTER_PID

	Lifter_SetLiftSpeed(lifterSpeed);

	// Disable the dumper tray if the lifter is not up far enough
	if (lifterPos < 700) {
		Lifter_SetTrayDumperExtended(
				Controller_IsLifterTrayDumpButtonPressed());
	} else {
		Lifter_SetTrayDumperExtended(false);
	}

	// Automatic tray tilting
	if (lifterPos < 910 && lifterPos > 740) {
		Lifter_SetTrayAngle(97);
	} else {
		Lifter_SetTrayAngle(127);
	}

	// Make sure the sweeper is not being controlled by the controller buttons
	// Kind of a stupid way to do it, but it works
	if (!Controller_IsSweeperInButtonPressed()
			&& !Controller_IsSweeperOutButtonPressed()) {
		// If the lifter is near the sweeper, spin it
		if (lifterPos > 650) {
			// Get the speed of the lifter pot, scale it to the range of a
			// joystick, and make sure it does not exceed valid motor range
			int lifterPotSpeed = Limit(Lifter_GetSpeed() * 4);
			Motor_Speed sweeperSpeed;
			// Choose the speed of the sweeper based on the stick value or
			// speed of the pot, whichever is larger.
			if (lifterPotSpeed < 0) {
				sweeperSpeed = Min(lifterPotSpeed, lifterSpeed / 2);
			} else {
				sweeperSpeed = Max(lifterPotSpeed, lifterSpeed / 2);
			}
			Sweeper_SetSpeed(-sweeperSpeed);
		} else {
			Sweeper_Stop();
		}
	}

#ifdef SMART_DASHBOARD
	SmartDashboard_putInt("Pot", lifterPos);
#endif // SMART_DASHBOARD
}

void Lifter_SetTrayDumperExtended(bool extended) {
	SetPWM(Lifter_trayDumperServoPort,
			extended ?
					Lifter_trayDumperPosition.extend :
					Lifter_trayDumperPosition.retract);
}

static void Lifter_SetPIDConstants(Lifter_PIDConstants* constants) {
	Lifter_PID.p = constants->p;
	Lifter_PID.i = constants->i;
	Lifter_PID.d = constants->d;
}

void Lifter_SetLiftSpeed(Motor_Speed speed) {
	Motor_set(Lifter_leftArmMotorPort, speed);
	Motor_set(Lifter_rightArmMotorPort, -speed);
}

unsigned int Lifter_GetPosition(void) {
	return GetAnalogInput(Lifter_potPort);
}

int Lifter_GetSpeed(void) {
	unsigned long time = GetMsClock();
	static unsigned long lastTime = 0;
	static unsigned int lastPos = 0;
	unsigned int pos = Lifter_GetPosition();
	static int speed = 0;

	if (time - lastTime > 50) {
		speed = pos - lastPos;
		lastPos = pos;
		lastTime = time;
	}

	return -speed;
}

void Lifter_SetTrayAngle(unsigned char angle) {
	static unsigned char currAngle = 127;
	static unsigned long lastIncrementTime = 0;
	unsigned long time = GetMsClock();

	if (time - lastIncrementTime > 5) {
		lastIncrementTime = time;

		if (currAngle < angle) {
			currAngle++;
		} else if (currAngle > angle) {
			currAngle--;
		}
	}

	SetPWM(Lifter_leftTrayServoPort, currAngle);
	SetPWM(Lifter_rightTrayServoPort, 255 - currAngle);
}
