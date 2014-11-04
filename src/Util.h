/* 
 * File:   Util.h
 * Author: Ben Wolsieffer
 *
 * Created on October 20, 2014, 9:59 PM
 */

#ifndef UTIL_H
#define	UTIL_H

/**
 * Computes the minimum of two numbers.
 * @param a one number
 * @param b another number
 * @return the minimum
 */
signed char min(char a, char b);

/**
 * Computes the maximum of two numbers.
 * @param a one number
 * @param b another number
 * @return the maximum
 */
signed char max(signed char a, signed char b);

unsigned char ConvertUnsigned(signed char n);

signed char ConvertSigned(unsigned char n);

#endif	/* UTIL_H */

