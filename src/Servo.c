#include "Servo.h"

void Servo_set(Servo_Port port, Servo_Position position) {
	SetPWM(port, position);
}

//Servo_Position Servo_get(Servo_Port port) {
//    return GetPWM(port);
//}
