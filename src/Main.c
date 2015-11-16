#include "Common.h"
#include "Controller.h"
#include "Drive.h"
#include "Sweeper.h"
#include "Autonomous.h"
#include "Motor.h"
#include "Lifter.h"
#include "SmartDashboard.h"

// Length of autonomous period in seconds
const unsigned char autonomousTime = 15;
// Length of teleoperated period in seconds
const unsigned char teleopTime = 120;

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
	SetCompetitionMode(autonomousTime, teleopTime);
#endif /* COMPETITION */
	// Set all ports to digital input mode
	DefineControllerIO(1, // Number of analog inputs
			INPUT, // 1 - digital I/O port mode
			INPUT, // 2
			INPUT, // 3
			INPUT, // 4
			OUTPUT, // 5
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
	OpenSerialPortOne(BAUD_115200);

	Drive_Init();
	Sweeper_Init();
	Lifter_Init();
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
 * switches to operator mode. If the field ever switches b3ack
 * to autonomous, then OperatorControl will automatically exit
 * and the program will transfer control to the Autonomous
 * function.
 */
void OperatorControl(void) {
	static SmartDashboard_Value sdValue;
	static unsigned long sdReceiveCount = 0;

	Drive_TeleopInit();
	Sweeper_TeleopInit();
	Lifter_TeleopInit();

	while (true) {
		Drive_Teleop();
		Sweeper_Update();
		Lifter_Update();

#ifdef SMART_DASHBOARD
		// Read SmartDashboard variables
		// This must be done from one location, because there is no way to get
		// a specific variable's value
		if (SmartDashboard_getValue(&sdValue)) {
			if (sdValue.type == INT) {
				int value = sdValue.value.integer;
//				if (SmartDashboard_keyEqual(&sdValue, Sweeper_pKey)) {
//					Sweeper_PID.p = value;
//				} else if (SmartDashboard_keyEqual(&sdValue, Sweeper_iKey)) {
//					Sweeper_PID.i = value;
//				} else if (SmartDashboard_keyEqual(&sdValue, Sweeper_dKey)) {
//					Sweeper_PID.d = value;
//				} else if (SmartDashboard_keyEqual(&sdValue, Sweeper_divKey)) {
//					Sweeper_PID.div = value;
//				}
				if (SmartDashboard_keyEqual(&sdValue, Lifter_velPDownKey)) {
					Lifter_velPIDDownConstants.p = value;
				} else if (SmartDashboard_keyEqual(&sdValue,
						Lifter_velIDownKey)) {
					Lifter_velPIDDownConstants.i = value;
				} else if (SmartDashboard_keyEqual(&sdValue,
						Lifter_velDDownKey)) {
					Lifter_velPIDDownConstants.d = value;
				} else if (SmartDashboard_keyEqual(&sdValue,
						Lifter_velPUpKey)) {
					Lifter_velPIDUpConstants.p = value;
				} else if (SmartDashboard_keyEqual(&sdValue,
						Lifter_velIUpKey)) {
					Lifter_velPIDUpConstants.i = value;
				} else if (SmartDashboard_keyEqual(&sdValue,
						Lifter_velDUpKey)) {
					Lifter_velPIDUpConstants.d = value;
				} else if (value
						&& SmartDashboard_keyEqual(&sdValue,
								Lifter_velDivKey)) {
					Lifter_velPID.div = value;
				} else if (value
						&& SmartDashboard_keyEqual(&sdValue,
								Lifter_velInputDivKey)) {
					Lifter_velInputDiv = value;
				} else {
					--sdReceiveCount;
				}
			} else {
				--sdReceiveCount;
			}
			SmartDashboard_putInt("RX", ++sdReceiveCount);
		}
#endif // SMART_DASHBOARD

#ifndef COMPETITION
		if (!IsEnabled())
			break;
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
	IO_Initialization();
	Initialize();
	while (true) {
		OperatorControl();
	}
#endif /* COMPETITION */
}
