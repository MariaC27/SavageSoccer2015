#include "Sweeper.h"

#include "Common.h"
#include "Controller.h"
#include "SmartDashboard.h"
#include "Util.h"

const Motor_Port Sweeper_motorPort = 7;
//
//static const unsigned char Sweeper_encoderChannelA = 1;
//static const unsigned char Sweeper_encoderChannelB = 2;
//
//static bool Sweeper_encoderLock = false;
//static long Sweeper_encoderPos = 0;
//
//PID Sweeper_PID;
//
//#ifdef SMART_DASHBOARD
//const rom char* Sweeper_pKey = "Sw_p";
//const rom char* Sweeper_iKey = "Sw_i";
//const rom char* Sweeper_dKey = "Sw_d";
//const rom char* Sweeper_divKey = "Sw_div";
//#endif // SMART_DASHBOARD

static const struct {
	Motor_Speed in;
	Motor_Speed out;
} Sweeper_motorSpeed = { -127, 127 };

//static void Sweeper_EncoderInterrupt(unsigned char port, unsigned char state) {
//	static long encoderPosPriv = 0;
//	if (GetDigitalInput(Sweeper_encoderChannelB)) {
//		encoderPosPriv++;
//	} else {
//		encoderPosPriv--;
//	}
//	if (!Sweeper_encoderLock) {
//		Sweeper_encoderPos = encoderPosPriv;
//	}
//}

//static int Sweeper_RestrictAngle(long angle) {
//	angle = angle % 360;
//
//	// force it to be the positive remainder, so that 0 <= angle < 360
//	angle = (angle + 360) % 360;
//
//	if (angle > 270) {
//		angle -= 360;
//	} else if (angle > 90) {
//		angle -= 180;
//	}
//
//	return angle;
//}

/**
 * Put code here to initialize the sweeper subsystem.
 */
void Sweeper_Init(void) {
//	StartQuadEncoder(Sweeper_encoderChannelA, Sweeper_encoderChannelB,
//			Sweeper_encoderInverted);
//	RegisterInterruptHandler(Sweeper_encoderChannelA, RISING_EDGE,
//			&Sweeper_EncoderInterrupt);

//	PID_Init(&Sweeper_PID);
//	Sweeper_PID.p = 44;
//	Sweeper_PID.i = 0;
//	Sweeper_PID.d = 70;
//	Sweeper_PID.div = 16;

//#ifdef SMART_DASHBOARD
//	SmartDashboard_putInt(Sweeper_pKey, Sweeper_PID.p);
//	SmartDashboard_putInt(Sweeper_iKey, Sweeper_PID.i);
//	SmartDashboard_putInt(Sweeper_dKey, Sweeper_PID.d);
//	SmartDashboard_putInt(Sweeper_divKey, Sweeper_PID.div);
//#endif // SMART_DASHBOARD
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
	static bool stopped = false;
	if (Controller_IsSweeperInButtonPressed()) {
		Sweeper_SweepIn();
		stopped = false;
	} else if (Controller_IsSweeperOutButtonPressed()) {
		Sweeper_SweepOut();
		stopped = false;
	} else {
		if (!stopped) {
			Sweeper_Stop();
			stopped = true;
		}
	}

#ifdef SMART_DASHBOARD
//	SmartDashboard_putInt("Enc", Sweeper_RestrictAngle(Sweeper_GetPosition()));
#endif // SMART_DASHBOARD

}

void Sweeper_SetSpeed(Motor_Speed speed) {
	Motor_set(Sweeper_motorPort, speed);
}

//void Sweeper_SetPosition(int angle, bool positiveDir) {
//	static int motorAngle = 0;
//	static int lastCurrAngle = 0;
//	static int lastAngle = 0;
//	static bool lastPositiveDir = true;
//	int currAngle = Sweeper_GetPosition();
//
//	if (lastAngle != angle || lastPositiveDir != positiveDir
//			|| abs(lastCurrAngle - currAngle) >= 360) {
//		motorAngle = Sweeper_RestrictAngle(angle);
//
//		lastAngle = angle;
//		lastPositiveDir = positiveDir;
//		lastCurrAngle = currAngle;
//
//		// Integer math!
//		motorAngle += ((currAngle - motorAngle) / 180) * 180;
//
//		if (positiveDir) {
//			if (motorAngle - currAngle < 0) {
//				motorAngle += 180;
//			}
//		} else {
//			if (motorAngle - currAngle > 0) {
//				motorAngle -= 180;
//			}
//		}
//	}
//
//	Motor_set(Sweeper_motorPort,
//			-PID_Calc(&Sweeper_PID, currAngle, motorAngle));
//}

//long Sweeper_GetPosition(void) {
//	long encoderPosTemp;
//	Sweeper_encoderLock = true;
//	encoderPosTemp = Sweeper_encoderPos;
//	Sweeper_encoderLock = false;
//	// 90 ticks per revolution
//	return encoderPosTemp * 4;
//}

void Sweeper_SweepIn(void) {
	Sweeper_SetSpeed(Sweeper_motorSpeed.in);
}

void Sweeper_SweepOut(void) {
	Sweeper_SetSpeed(Sweeper_motorSpeed.out);
}

void Sweeper_Stop(void) {
	Motor_set(Sweeper_motorPort, 0);
}
