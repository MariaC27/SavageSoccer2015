#include "Motor.h"

void Motor_set(Motor_Port port, Motor_Speed speed) {
    SetPWM(port, 127 + speed);
}

//Motor_Speed Motor_get(Motor_Port port) {
//    return GetPWM(port) - 127;
//}