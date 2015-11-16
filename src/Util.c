#include "Util.h"
#include "Common.h"

unsigned char RAMPER_TYPE_UP = 0x1;
unsigned char RAMPER_TYPE_DOWN = 0x2;

int Ramper_Ramp(Ramper* ramper, int input) {
	// Get current time
	unsigned long time = GetMsClock();

	// Check if enough time has passed to ramp
	if (time - ramper->lastTime > ramper->interval) {
		// Check if ramper is set up for up, down or both
		bool up = ramper->type & RAMPER_TYPE_UP == RAMPER_TYPE_UP;
		bool down = ramper->type & RAMPER_TYPE_DOWN == RAMPER_TYPE_DOWN;

		// Change since last calculation
		int delta = input - ramper->lastOutput;
		int output = input;

		// If the value increased too fast, limit it
		if (Abs(delta) > ramper->maxDelta) {
			// Logic to determine how to adjust the output value
			if (delta < 0) {
				if (input < 0) {
					if (up) {
						output = ramper->lastOutput - ramper->maxDelta;
					}
				} else {
					if (down) {
						output = ramper->lastOutput + ramper->maxDelta;
					}
				}
			} else {
				if (input > 0) {
					if (up) {
						output = ramper->lastOutput + ramper->maxDelta;
					}
				} else {
					if (down) {
						output = ramper->lastOutput - ramper->maxDelta;
					}
				}
			}
		}
		// Save output value and time
		ramper->lastOutput = output;
		ramper->lastTime = time;
	}

	return ramper->lastOutput;
}

signed char Min(signed char a, signed char b) {
	return (a < b) ? a : b;
}

signed char Max(signed char a, signed char b) {
	return (a > b) ? a : b;
}

int Abs(int n) {
	return n * ((n < 0) ? -1 : 1);
}

signed char Limit(int i) {
	if (i > 127) {
		return 127;
	} else if (i < -127) {
		return -127;
	} else {
		return i;
	}
}

unsigned char ConvertUnsigned(signed char n) {
	return (short) n + 127;
}

signed char ConvertSigned(unsigned char n) {
	if (n == 255)
		n--;
	return (short) n - 127;
}

