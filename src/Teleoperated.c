#include "Teleoperated.h"
#include "Drive.h"
#include "Dumper.h"

void Teleop_Init(void) {
    Drive_TeleopInit();
    Dumper_TeleopInit();
}

void Teleop_Update(void) {
    Drive_Teleop();
    Dumper_Update();
}
