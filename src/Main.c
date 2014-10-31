#include "Common.h"
#include "Controller.h"
#include "Drive.h"
#include "Dumper.h"
#include "Autonomous.h"
#include "Teleoperated.h"

// Length of autonomous period in seconds
#define AUTONOMOUS_TIME 10
// Length of teleoperated period in seconds
#define TELEOP_TIME 120

void IO_Initialization(void);
void Initialize(void);
void OperatorControl(void);
void main(void);

/*
 * This function must be here for a competition project. It
 * is automatically referenced by WPILib at startup and run.
 * At that point the SetCompetitionMode function sets the
 * competition mode. Basically, a mode of 0 is the default
 * (without the IO_Initialization function) and runs a main
 * function only.
 * SetCompetitionMode(1) runs a competition project as shown.
 */
void IO_Initialization(void) {
#ifdef COMPETITION
    // Ten second autonomous and 120 second teleop
    SetCompetitionMode(AUTONOMOUS_TIME, TELEOP_TIME);
#endif /* COMPETITION */
    // Set all ports to digital input mode
    DefineControllerIO(
            0, // Number of analog inputs
            INPUT, // 1 - digital I/O port mode
            INPUT, // 2
            INPUT, // 3
            INPUT, // 4
            INPUT, // 5
            INPUT, // 6
            INPUT, // 7
            INPUT, // 8
            INPUT, // 9
            INPUT, // 10
            INPUT, // 11
            INPUT, // 12
            INPUT, // 13
            INPUT, // 14
            INPUT, // 15
            INPUT //  16
            );
}

/*
 * Initialize is run immediately when the robot is powered on
 * regardless of the field mode.
 */
void Initialize(void) {
    Drive_Init();
    Dumper_Init();
}

/*
 * Autonomous is run as soon as the field controls enable the
 * robot. At the end of the autonomous period, the Autonomous
 * function will end (note: even if it is in an infinite loop
 * as in the example, it will be stopped).
 */
void Autonomous(void) {
    Autonomous_Run();
}

/*
 * The OperatorControl function will be called when the field
 * switches to operator mode. If the field ever switches back
 * to autonomous, then OperatorControl will automatically exit
 * and the program will transfer control to the Autonomous
 * function.
 */
void OperatorControl(void) {
    Teleop_Init();
    while (true) {
        Teleop_Update();
#ifndef COMPETITION
        if (!IsEnabled()) break;
#endif /* COMPETITION */
    }
}

/*
 * This main() function is run when the program is compiled to run in 
 * non-competition mode. It just runs teleop so we can do demos without worrying
 * about autonomous or the match ending.
 */
void main(void) {
#ifndef COMPETITION
    while (TRUE) {
        OperatorControl();
        while (!IsEnabled());
    }
#endif /* COMPETITION */
}
