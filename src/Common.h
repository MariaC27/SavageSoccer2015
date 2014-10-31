/* 
 * File:   Common.h
 * Author: Ben Wolsieffer
 *
 * Created on November 11, 2013, 9:17 AM
 */

#ifndef COMMON_H
#define	COMMON_H

#include <Api.h>

/**
 * Enables competition mode. This makes the robot run autonomous and stop teleop
 * at the end of the match. Comment this out for demos, etc.
 */
#define COMPETITION

/**
 * Boolean data type. This is done for convenience and semantic clarity because 
 * C does not have a bool type.
 */
typedef enum {
    false = 0, true = 1
} bool;

#endif	/* COMMON_H */