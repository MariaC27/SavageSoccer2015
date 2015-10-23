#include "Autonomous.h"
#include "Drive.h"

/*
 * This is where our autonomous code goes. Note: this is not run in a loop like
 * the teleop code, but instead it runs once and all delays should be done with
 * the Wait(millis) function.
 */
void Autonomous_Run(void) {
	Drive_AutonomousInit();
}
