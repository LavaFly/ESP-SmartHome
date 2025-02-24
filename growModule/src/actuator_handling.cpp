#include "actuator_handling.h"
uint8_t actuatorActive = 0;

void initActuator(){
    pinMode(PUMP_POWER, OUTPUT);
    digitalWrite(PUMP_POWER, LOW);
}

bool getActuatorStatus(){
    return actuatorActive;
}

void activateActuator(){
    digitalWrite(PUMP_POWER, HIGH);
    actuatorActive = 1;
}

void deactivateActuator(){
    digitalWrite(PUMP_POWER, LOW);
    actuatorActive = 0;
}

