#include "actuator_handling.h"

// light relay stuff
// lcd display stuff
void initActuator(){
}

bool getActuatorStatus(){
    return digitalRead(PUMP_POWER);
}

void activateActuator(){
    digitalWrite(PUMP_POWER, HIGH);
}

void deactivateActuator(){
    digitalWrite(PUMP_POWER, LOW);
}

