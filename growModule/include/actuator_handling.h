#ifndef ACTUATOR_HANDLING
#define ACTUATOR_HANDLING

#include <Arduino.h>

#define PUMP_POWER 13 // D7

/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
uint8_t initActuator();


/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
uint8_t getActuatorStatus();


/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
uint8_t activateActuator();


/**
 * @brief
 *
 * @param
 */
void deactivateActuator();

#endif
