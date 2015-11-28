#include "SmartDashboard.h"

#define CHAR_BEG 0x02
#define  CHAR_SEP 0x03
#define CHAR_ESC 0x04
#define CHAR_END 0x05

#define PACKET_BUFFER_SIZE 100
#define MAX_NAME_SIZE 20

#ifdef SMART_DASHBOARD

static int packetBufferPosition = 0;
static int packetBufferLength = 0;
// Length must be 256, because getValue() uses char overflow
static unsigned char packetBuffer[PACKET_BUFFER_SIZE];
static char nameBuffer[MAX_NAME_SIZE];
// Must be increased to support larger data types
static unsigned char valueBuffer[5];

static bool SmartDashboard_needsEscape(const char c) {
	return c == CHAR_BEG || c == CHAR_END || c == CHAR_ESC || c == CHAR_SEP;
}

static void SmartDashboard_sendKey(rom const char* key) {
	WriteSerialPortOne(CHAR_BEG);
	while ((*key) != 0) {
		const char c = *(key++);
		if (SmartDashboard_needsEscape(c)) {
			WriteSerialPortOne(CHAR_ESC);
		}
		WriteSerialPortOne(c);
	}
	WriteSerialPortOne(CHAR_SEP);
}
#endif // SMART_DASHBOARD

void SmartDashboard_putInt(rom const char* key, const long value) {
#ifdef SMART_DASHBOARD
	char s;
	SmartDashboard_sendKey(key);

	WriteSerialPortOne(INT);
	for (s = 24; s >= 0; s -= 8) {
		char data = (unsigned char) (value >> s);
		if (SmartDashboard_needsEscape(data)) {
			WriteSerialPortOne(CHAR_ESC);
		}
		WriteSerialPortOne(data);
	}

	WriteSerialPortOne(CHAR_END);
#endif // SMART_DASHBOARD
}

void SmartDashboard_putBool(rom const char* key, const bool value) {
#ifdef SMART_DASHBOARD
	SmartDashboard_sendKey(key);
	WriteSerialPortOne(BOOL);
	WriteSerialPortOne(value ? 1 : 0);
	WriteSerialPortOne(CHAR_END);
#endif // SMART_DASHBOARD
}

bool SmartDashboard_getValue(SmartDashboard_Value* value) {
#ifdef SMART_DASHBOARD
	int readSize = GetSerialPort1ByteCount();
	// Counter variable
	int i;
	// Flag storing whether a packet has started being read
	bool began = false;
	// Flag indicating whether the previous byte was an escape
	bool esc = false;

	// Current position in the buffers (-1 because they are incremented before
	// being used)
	int namePosition = -1;
	int valuePosition = -1;
	// Flag indicating which section of the packet is being read
	bool valueSection = false;
	unsigned char newData;
	bool readEndChar = false;

	// If the packet buffer can't hold all serial reduce the amount that will
	// be read
	if (packetBufferLength + readSize > PACKET_BUFFER_SIZE) {
		readSize = PACKET_BUFFER_SIZE - packetBufferLength;
	}

	for (i = 0; i < readSize; i++) {
		newData = ReadSerialPortOne();
		packetBuffer[(packetBufferPosition + packetBufferLength++)
				% PACKET_BUFFER_SIZE] = newData;
		if (newData == CHAR_END) {
			readEndChar = true;
		}
//		PrintToScreen("%u\n", (unsigned int) newData);
	}

	if (readEndChar) {
//		PrintToScreen("rEC\n");
		// Process packet buffer
		for (i = 0; i < packetBufferLength; i++) {
			// Get next byte from buffer
			unsigned char data = packetBuffer[(packetBufferPosition + i)
					% PACKET_BUFFER_SIZE];

//			PrintToScreen("pBL: %i, pBP: %i, i: %i, d: %u\n",
//					packetBufferLength, packetBufferPosition, i,
//					(unsigned int) data);

			// If the previous character wasn't an escape, process special
			// characters
			if (!esc) {
				switch (data) {
				case CHAR_ESC:
					// Escape character: set escape flag
					esc = true;
					break;
				case CHAR_BEG:
					// Begin character: reset buffers and flags because a new packet
					// is starting
					began = true;
					namePosition = -1;
					valuePosition = -1;
					valueSection = false;
					break;
				case CHAR_SEP:
					// Separator character: switch to the value section
					if (began) {
						valueSection = true;
					}
					break;
				case CHAR_END:
					// End character: the entire packet should have been read, so
					// process it
					if (began) {
						unsigned char* j;
						// Append null character to end of name
						nameBuffer[++namePosition] = '\0';
						value->name = nameBuffer;
						value->type = valueBuffer[0];
						packetBufferLength -= (i + 1);
						packetBufferPosition += (i + 1);
						packetBufferPosition %= PACKET_BUFFER_SIZE;

						began = false;

//						PrintToScreen("type: %u\n", (unsigned int) value->type);
//						PrintToScreen("value: ");
//						for (j = valueBuffer; j < valueBuffer + 5; j++) {
//							PrintToScreen("%i", (unsigned int) *j);
//						}
//						PrintToScreen("\n");

						switch (value->type) {
						case INT:
							value->value.integer = *((long*) (valueBuffer + 1));
							break;
						case BOOL:
							value->value.boolean =
									valueBuffer[1] ? true : false;
						default:
							return false;
						}

//						PrintToScreen("process: %i\n", value->value.integer);

						return true;
					}
					break;
				default:
					if (began) {
						// Gasp... http://xkcd.com/292/
						goto append_byte;
					}
				}
			} else {
				if (began) {
					esc = false;

					append_byte:
//					PrintToScreen("appb\n");

					if (valueSection) {
						if (++valuePosition < 5) {
							valueBuffer[valuePosition] = data;
//							PrintToScreen("valApp: %u pos: %i\n",
//									(unsigned int) valueBuffer[valuePosition],
//									valuePosition);
						}
					} else {
						if (++namePosition < MAX_NAME_SIZE - 1) {
							nameBuffer[namePosition] = data;
//							PrintToScreen("namApp: %u pos: %i\n",
//									(unsigned int) nameBuffer[namePosition],
//									namePosition);
						}
					}
				}
			}
			if (!began && i == (packetBufferLength - 1)) {
//				PrintToScreen("Pos++\n");
				packetBufferLength -= (i + i);
				packetBufferPosition += (i + i);
				packetBufferPosition %= PACKET_BUFFER_SIZE;
			}
		}
	}
#endif // SMART_DASHBOARD

	return false;
}

bool SmartDashboard_keyEqual(SmartDashboard_Value* value, const rom char* key) {
#ifdef SMART_DASHBOARD

	char* v = value->name;
	rom char* k = key;
	while (true) {
		if (*v != *k) {
			return false;
		}
		if (!(*v && *k)) {
			return true;
		}
		++v;
		++k;
	}
#else // SMART_DASHBOARD
	return false;
#endif
}
