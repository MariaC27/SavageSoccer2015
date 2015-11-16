#include "Sweeper.h"

#include "Common.h"
#include "Controller.h"
#include "SmartDashboard.h"
#include "Util.h"

const Motor_Port Sweeper_motorPort = 7;

static const struct {
	Motor_Speed in;
	Motor_Speed out;
} Sweeper_motorSpeed = { -127, 127 };

/**
 * Put code here to initialize the sweeper subsystem.
 */
void Sweeper_Init(void) {
}

/**
 * Put code here to initialize the sweeper at the beginning of teleop.
 */
void Sweeper_TeleopInit(void) {
}

/**
 * Put code here to update the sweeper based on the controls.
 */
void Sweeper_Update(void) {
	// Store whether the sweeper has been stopped
	static bool stopped = false;

	if (Controller_IsSweeperInButtonPressed()) {
		Sweeper_SweepIn();
		stopped = false;
	} else if (Controller_IsSweeperOutButtonPressed()) {
		Sweeper_SweepOut();
		stopped = false;
	} else {
		// Only stop the sweeper once, so the lifter auto rotate code can work
		if (!stopped) {
			Sweeper_Stop();
			stopped = true;
		}
	}

}

void Sweeper_SetSpeed(Motor_Speed speed) {
	Motor_set(Sweeper_motorPort, speed);
}

void Sweeper_SweepIn(void) {
	Sweeper_SetSpeed(Sweeper_motorSpeed.in);
}

void Sweeper_SweepOut(void) {
	Sweeper_SetSpeed(Sweeper_motorSpeed.out);
}

void Sweeper_Stop(void) {
	Sweeper_SetSpeed(0);
}
