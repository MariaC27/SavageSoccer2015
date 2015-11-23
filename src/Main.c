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

// Forward declarations
void IO_Initialization(void);
void Initialize(void);
void OperatorControl(void);
void main(void);

/**
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
	DefineControllerIO(1, // Number of analog inputs (for lifter pot)
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
			INPUT, // 14 - Autonomous config
			INPUT, // 15 - Autonomous config
			INPUT //  16 - Autonomous config
			);
}

/**
 * Initialize is run immediately when the robot is powered on
 * regardless of the field mode.
 */
void Initialize(void) {
	// This seems to make reading from the serial port work, even though it is
	// supposed to be opened automatically.
	OpenSerialPortOne(BAUD_115200);

	// Initialize subsystems
	Drive_Init();
	Sweeper_Init();
	Lifter_Init();
}

/**
 * If in competition mode, this function runs as soon as the controller is
 * connected and stops after a preset amount of time. When not in competition
 * mode, this function does not run.
 */
void Autonomous(void) {
	// Run autonomous mode
	Autonomous_Run();
}

/**
 * The OperatorControl function will be called when the field switches to
 * teleoperated mode. If the code was configured in competition mode, this runs
 * after autonomous finishes, and will automatically stop when the match ends.
 * Otherwise, this function is called after the robot is initialized and runs
 * until the robot is turned off.
 */
void OperatorControl(void) {
	static SmartDashboard_Value sdValue;
	// Count of number of known values received from the SmartDashboard. Used
	// to echo back a receive confirmation.
	static unsigned long sdReceiveCount = 0;

	// Initialize subsystems at the beginning of teleop
	Drive_TeleopInit();
	Sweeper_TeleopInit();
	Lifter_TeleopInit();

	// Run teleop update loop
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
			// Write the receive counter to the SmartDashboard, to give
			// confirmation that the message was received correctly.
			SmartDashboard_putInt("RX", ++sdReceiveCount);
		}
#endif // SMART_DASHBOARD
	}
}

/**
 * This main() function is run when the program is compiled to run in 
 * non-competition mode. It just runs teleop so we can do demos without worrying
 * about autonomous or the match ending.
 */
void main(void) {
#ifndef COMPETITION
	// Initialize digital and analog inputs
	IO_Initialization();
	// Initialize the robot
	Initialize();
	// Run the teleop loop
	OperatorControl();
#endif /* COMPETITION */
}
