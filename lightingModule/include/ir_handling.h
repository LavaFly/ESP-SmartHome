#ifndef IR_HANDLING
#define IR_HANDLING

#include <Arduino.h>

#define IR_ADDRESS      0x80
#define LIGHT_ON        0x12
#define LIGHT_OFF       0x1e
#define LIGHT_BRIGHTER  0x0e
#define LIGHT_DARKER    0x0c

uint8_t buildIrConnection();
void sendOffSignal();
void sendOnSignal();
void sendBrighterSignal();
void sendDarkerSignal();
void sendSignal();
uint8_t checkForNewMessage();

#endif
