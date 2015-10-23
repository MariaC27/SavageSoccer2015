#include "Motor.h"
#include "Util.h"

void Motor_set(Motor_Port port, Motor_Speed speed) {
	SetPWM(port, ConvertUnsigned(speed));
	//    SetPWM(port, speed + 127);
}

//Motor_Speed Motor_get(Motor_Port port) {
//    return GetPWM(port) - 127;
//}
