#include "actuator_handling.h"
uint8_t actuatorActive = 0;

uint8_t initActuator(){
    pinMode(PUMP_POWER, OUTPUT);
    digitalWrite(PUMP_POWER, LOW);
    // this will get better at some point
    return 1;
}

uint8_t getActuatorStatus(){
    return actuatorActive;
}

uint8_t activateActuator(){
    digitalWrite(PUMP_POWER, HIGH);
    actuatorActive = 1;
    return 1;
}

uint8_t deactivateActuator(){
    digitalWrite(PUMP_POWER, LOW);
    actuatorActive = 0;
    return 1;
}

