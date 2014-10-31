/* 
 * File:   Teleoperated.h
 * Author: ben
 *
 * Created on October 19, 2014, 8:33 PM
 */

#ifndef TELEOPERATED_H
#define	TELEOPERATED_H

/**
 * Runs at the beginning of teleop.
 */
void Teleop_Init(void);

/**
 * Runs in a loop during teleop.
 */
void Teleop_Update(void);

#endif	/* TELEOPERATED_H */

