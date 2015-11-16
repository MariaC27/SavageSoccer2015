/* 
 * File:   Util.h
 * Author: Ben Wolsieffer
 *
 * Created on October 20, 2014, 9:59 PM
 */

#ifndef UTIL_H
#define	UTIL_H

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

