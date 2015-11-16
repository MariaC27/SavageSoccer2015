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
 * competition mode. The parameters control the length of
 * autonomous and teleop.
 */
void IO_Initialization(void) {
	// COMPETITION is defined in Common.h
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
	// This seems to make reading from the serial port work
	OpenSerialPortOne(BAUD_115200);

	// Initialize subsystems
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
#ifdef LIFTER_PID
				if (SmartDashboard_keyEqual(&sdValue, Lifter_pDownKey)) {
					Lifter_PIDDownConstants.p = value;
				} else if (SmartDashboard_keyEqual(&sdValue, Lifter_iDownKey)) {
					Lifter_PIDDownConstants.i = value;
				} else if (SmartDashboard_keyEqual(&sdValue, Lifter_dDownKey)) {
					Lifter_PIDDownConstants.d = value;
				} else if (SmartDashboard_keyEqual(&sdValue, Lifter_pUpKey)) {
					Lifter_PIDUpConstants.p = value;
				} else if (SmartDashboard_keyEqual(&sdValue, Lifter_iUpKey)) {
					Lifter_PIDUpConstants.i = value;
				} else if (SmartDashboard_keyEqual(&sdValue, Lifter_dUpKey)) {
					Lifter_PIDUpConstants.d = value;
				} else if (value
						&& SmartDashboard_keyEqual(&sdValue, Lifter_divKey)) {
					Lifter_PID.div = value;
				} else
#endif // LIFTER_PID
				{
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
