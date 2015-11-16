#include "Lifter.h"
#include "Motor.h"
#include "Controller.h"
#include "SmartDashboard.h"
#include "Sweeper.h"
#include "Util.h"

static void Lifter_SetVelPIDConstants(Lifter_PIDConstants* constants);

const Motor_Port Lifter_leftArmMotorPort = 3;
const Motor_Port Lifter_rightArmMotorPort = 4;
const unsigned char Lifter_leftTrayServoPort = 5;
const unsigned char Lifter_rightTrayServoPort = 6;
const unsigned char Lifter_trayDumperServoPort = 8;

const unsigned char Lifter_potPort = 1;

PID Lifter_velPID;
Lifter_PIDConstants Lifter_velPIDDownConstants;
Lifter_PIDConstants Lifter_velPIDUpConstants;
int Lifter_velInputDiv = 4;

#ifdef SMART_DASHBOARD
const rom char* Lifter_velPDownKey = "LvPd";
const rom char* Lifter_velIDownKey = "LvId";
const rom char* Lifter_velDDownKey = "LvDd";
const rom char* Lifter_velPUpKey = "LvPu";
const rom char* Lifter_velIUpKey = "LvIu";
const rom char* Lifter_velDUpKey = "LvDu";
const rom char* Lifter_velDivKey = "LvDiv";
const rom char* Lifter_velInputDivKey = "LvInDiv";
#endif // SMART_DASHBOARD

const struct {
	unsigned char retract;
	unsigned char extend;
} Lifter_trayDumperPosition = { 180, 0 };

/**
 * Put code here to initialize the lifter subsystem.
 */
void Lifter_Init(void) {
	PID_Init(&Lifter_velPID);
	Lifter_velPIDDownConstants.p = 0;
	Lifter_velPIDDownConstants.i = 0;
	Lifter_velPIDDownConstants.d = 0;
	Lifter_velPIDUpConstants.p = 0;
	Lifter_velPIDUpConstants.i = 0;
	Lifter_velPIDUpConstants.d = 0;
	Lifter_velPID.div = 16;

	Lifter_SetVelPIDConstants(&Lifter_velPIDUpConstants);

	Lifter_SetTrayDumperExtended(false);

#ifdef SMART_DASHBOARD
	SmartDashboard_putInt(Lifter_velPDownKey, Lifter_velPIDDownConstants.p);
	SmartDashboard_putInt(Lifter_velIDownKey, Lifter_velPIDDownConstants.i);
	SmartDashboard_putInt(Lifter_velDDownKey, Lifter_velPIDDownConstants.d);
	SmartDashboard_putInt(Lifter_velPUpKey, Lifter_velPIDUpConstants.p);
	SmartDashboard_putInt(Lifter_velIUpKey, Lifter_velPIDUpConstants.i);
	SmartDashboard_putInt(Lifter_velDUpKey, Lifter_velPIDUpConstants.d);
	SmartDashboard_putInt(Lifter_velDivKey, Lifter_velPID.div);
	SmartDashboard_putInt(Lifter_velInputDivKey, Lifter_velInputDiv);
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
	static unsigned long lastLifterSpeedTime = 0;
	unsigned long time = GetMsClock();
	unsigned int lifterPos = Lifter_GetPosition();
	static Controller_Position lastLifterSpeed = 0;
	Controller_Position lifterSpeed = Controller_GetLifterSpeed() / 2;

	if (time - lastLifterSpeedTime > 5) {
		int lifterSpeedDelta = lifterSpeed - lastLifterSpeed;

		// If the value increased too fast, limit it
		if (Abs(lifterSpeedDelta) > 1) {
			if (lifterSpeedDelta < 0) {
				if (lifterSpeed < 0) {
					lifterSpeed = lastLifterSpeed - 1;
				}
			} else {
				if (lifterSpeed > 0) {
					lifterSpeed = lastLifterSpeed + 1;
				}
			}
		}
		lastLifterSpeed = lifterSpeed;
		lastLifterSpeedTime = time;
	}

	Lifter_SetLiftSpeed(lifterSpeed);

	if (lifterPos < 700) {
		Lifter_SetTrayDumperExtended(
				Controller_IsLifterTrayDumpButtonPressed());
	} else {
		Lifter_SetTrayDumperExtended(false);
	}

	if (lifterPos < 910 && lifterPos > 740) {
		Lifter_SetTrayAngle(97);
	} else {
		Lifter_SetTrayAngle(127);
	}

	if (!Controller_IsSweeperInButtonPressed()
			&& !Controller_IsSweeperOutButtonPressed()) {
		if (lifterPos > 650) {
			int lifterPotSpeed = Limit(Lifter_GetSpeed() * 4);
			Motor_Speed sweeperSpeed;
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

static void Lifter_SetVelPIDConstants(Lifter_PIDConstants* constants) {
	Lifter_velPID.p = constants->p;
	Lifter_velPID.i = constants->i;
	Lifter_velPID.d = constants->d;
}

void Lifter_SetLiftSpeed(Motor_Speed speed) {
	unsigned long time = GetMsClock();
	unsigned int pos = Lifter_GetPosition();
	int lifterSpeed = Lifter_GetSpeed();

	if (pos > 420) {
		if (speed < 0) {
			Lifter_SetVelPIDConstants(&Lifter_velPIDDownConstants);
		} else {
			Lifter_SetVelPIDConstants(&Lifter_velPIDUpConstants);
		}
	} else {
		if (speed > 0) {
			Lifter_SetVelPIDConstants(&Lifter_velPIDDownConstants);
		} else {
			Lifter_SetVelPIDConstants(&Lifter_velPIDUpConstants);
		}
	}

#ifdef SMART_DASHBOARD
	SmartDashboard_putInt("IS", speed / Lifter_velInputDiv);
	SmartDashboard_putInt("LS", lifterSpeed);
#endif // SMART_DASHBOARD

	speed += PID_Calc(&Lifter_velPID, lifterSpeed, speed / Lifter_velInputDiv);

#ifdef SMART_DASHBOARD
	SmartDashboard_putInt("OS", speed);
#endif // SMART_DASHBOARD

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
