#ifndef SMARTDASHBOARD_H
#define	SMARTDASHBOARD_H

#include "Common.h"

typedef enum {
	INT = 0x06, BOOL = 0x07
} SmartDashboard_Type;

typedef struct {
	char type;
	char* name;
	union {
		long integer;
		bool boolean;
	} value;
} SmartDashboard_Value;

/**
 * Put an integer on to the SmartDashboard.
 *
 * @param key the name of the value
 * @param value the integer value
 */
void SmartDashboard_putInt(rom const char* key, const long value);

/**
 * Put a boolean on to the SmartDashboard.
 *
 * @param key the name of the value
 * @param value the boolean value
 */
void SmartDashboard_putBool(rom const char* key, const bool value);

bool SmartDashboard_getValue(SmartDashboard_Value* value);

bool SmartDashboard_keyEqual(SmartDashboard_Value* value, const rom char* key);

#endif	/* SMARTDASHBOARD_H */
