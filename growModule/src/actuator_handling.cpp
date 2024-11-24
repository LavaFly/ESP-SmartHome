#include "actuator_handling.h"

void initActuator(){
    pinMode(PUMP_POWER, OUTPUT);
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

