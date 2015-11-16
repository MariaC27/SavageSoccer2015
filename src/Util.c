#include "Util.h"

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
