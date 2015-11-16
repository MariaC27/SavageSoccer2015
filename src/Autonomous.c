#include "Autonomous.h"
#include "Drive.h"
#include "Sweeper.h"

struct {
	unsigned char startPin;
	unsigned char numPins;
} Autonomous_config = { 10, 3 };

static void Autonomous_CubeSweep() {
	// Turn on sweeper
	Sweeper_SweepIn();
	Drive_Straight(127);
	Wait(3000);
	Drive_Stop();
	Sweeper_Stop();
}

/*
 * This is where our autonomous code goes. Note: this is not run in a loop like
 * the teleop code, but instead it runs once and all delays should be done with
 * the Wait(millis) function.
 */
void Autonomous_Run(void) {
	unsigned char autoMode = 0;
	unsigned char i;

	Drive_AutonomousInit();

	// Read autonomous configuration pins
	for (i = 0; i < Autonomous_config.numPins; i++) {
		if (GetDigitalInput(Autonomous_config.startPin + i)) {
			autoMode |= 1 << i;
		}
	}

	switch (autoMode) {
	case 0b1:
		Autonomous_CubeSweep();
		break;
	}

}

