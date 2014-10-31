/* 
 * File:   Dumper.h
 * Author: Ben Wolsieffer
 *
 * Created on November 11, 2013, 1:37 PM
 */

#ifndef DUMPER_H
#define	DUMPER_H

#include "Common.h"

typedef enum {
    MIDDLE_OPEN_END_CLOSED,
    MIDDLE_CLOSED_END_CLOSED,
    MIDDLE_CLOSED_END_OPEN,
    MIDDLE_OPEN_END_OPEN
} Dumper_DoorState;

/**
 * Runs once at the beginning of teleop.
 */
void Dumper_Init(void);

/**
 * Runs once at the beginning of teleop.
 */
void Dumper_TeleopInit(void);

/**
 * Runs in a loop during teleop.
 */
void Dumper_Update(void);

/**
 * Set the state of the dumper doors.
 * 
 * @param ds the state of the doors
 */
void Dumper_SetDoorState(Dumper_DoorState ds);

/**
 * Start raising the dumper.
 */
void Dumper_Raise(void);

/**
 * Start lowering the dumper.
 */
void Dumper_Lower(void);

/**
 * Stop the dumper.
 */
void Dumper_Stop(void);

#endif	/* DUMPER_H */

