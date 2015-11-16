/* 
 * File:   Util.h
 * Author: Ben Wolsieffer
 *
 * Created on October 20, 2014, 9:59 PM
 */

#ifndef UTIL_H
#define	UTIL_H

/**
 * Bit mask specifying that the ramper should ramp up.
 */
extern unsigned char RAMPER_TYPE_UP;
/**
 * Bit mask specifying that the ramper should ramp down.
 */
extern unsigned char RAMPER_TYPE_DOWN;

/**
 * Holds data necessary for the ramping algorithm
 */
typedef struct {
	// Public
	unsigned long interval;
	int maxDelta;
	/**
	 * RAMPER_TYPE_UP, RAMPER_TYPE_DOWN or a bitwise or of the two.
	 */
	unsigned char type;

	// Private
	unsigned long lastTime;
	int lastOutput;
} Ramper;

/**
 * Ramps the input value using the parameters specified in the Ramper struct.
 */
int Ramper_Ramp(Ramper* ramper, int input);

/**
 * Computes the minimum of two bytes.
 * @param a one number
 * @param b another number
 * @return the minimum
 */
signed char Min(char a, char b);

/**
 * Computes the maximum of two bytes.
 * @param a one number
 * @param b another number
 * @return the maximum
 */
signed char Max(signed char a, signed char b);

int Abs(int n);

signed char Limit(int i);

unsigned char ConvertUnsigned(signed char n);

signed char ConvertSigned(unsigned char n);

#endif	/* UTIL_H */

