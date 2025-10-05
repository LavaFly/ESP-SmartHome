#ifndef TOUCH_HANDLING
#define TOUCH_HANDLING

#include <Arduino.h>

#define NUM_BUTTONS  2
#define BUTTON_PIN1 13
#define BUTTON_PIN2 12


void handleButtonInput(uint8_t indexInButtonList);
void updateStateMachine(uint8_t indexInButtonList);
uint8_t setupButtons();
void touchLoop();

#endif
