#ifndef ACTUATOR_HANDLING
#define ACTUATOR_HANDLING

#include <Arduino.h>

#define PUMP_POWER 13 // D7

void initActuator();
bool getActuatorStatus();
void activateActuator();
void deactivateActuator();

#endif
