#include "Util.h"

signed char min(signed char a, signed char b) {
    return (a < b) ? a : b;
}

signed char max(signed char a, signed char b) {
    return (a > b) ? a : b;
}

int abs(int n) {
    return n * ((n < 0) ? -1 : 1);
}

unsigned char ConvertUnsigned(signed char n) {
    return (short) n + 127;
}

signed char ConvertSigned(unsigned char n) {
    if (n == 255) n--;
    return (short) n - 127;
}