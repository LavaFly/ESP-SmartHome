#ifndef TOUCH_HANDLING
#define TOUCH_HANDLING

#include <Arduino.h>

void sendOffSignal();
void sendOnSignal();
void sendBrighterSignal();
void sendDarkerSignal();
void sendSignal();
uint8_t checkForNewMessage();
uint8_t checkForEvent();

#endif
