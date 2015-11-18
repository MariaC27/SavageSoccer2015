#include "Autonomous.h"
#include "Drive.h"
#include "Sweeper.h"

struct {
	unsigned char startPin;
	unsigned char numPins;
} Autonomous_config = { 14, 3 };

static void Autonomous_CubeSweep(void) {
	unsigned char i = 0;

	// Turn on sweeper
	Sweeper_SweepIn();

	// Drive forward to the cube
	Drive_Tank(50, 70);
	Wait(1600);
	Drive_Stop();

	// Wait 0.75 seconds for the cubes to be picked up
	Wait(750);

	// Wiggle the robot back and forth
	for (i = 0; i < 4; i++) {
		Drive_Arcade(0, -70);
		Wait(300);
		Drive_Arcade(0, 70);
		Wait(300);
	}
	Drive_Stop();

	// Stop the sweeper
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

	// Read autonomous configuration pins
	for (i = 0; i < Autonomous_config.numPins; i++) {
		// Digital inputs have pull-up resistors, and are pulled down when a
		// jumper is connected.
		if (!GetDigitalInput(Autonomous_config.startPin + i)) {
			autoMode |= 1 << i;
		}
	}

//	PrintToScreen("%i", (int) autoMode);

	switch (autoMode) {
	case 1:
		Autonomous_CubeSweep();
		break;
	}

}

