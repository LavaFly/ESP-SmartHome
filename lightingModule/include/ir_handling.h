#ifndef IR_HANDLING
#define IR_HANDLING

#include <Arduino.h>

#define IR_SEND_PIN     12
#define IR_ADDRESS      0x80
#define LIGHT_ON        0x12
#define LIGHT_OFF       0x1e
#define LIGHT_BRIGHTER  0x0e
#define LIGHT_DARKER    0x0c

void buildIrConnection();
void sendOffSignal();
void sendOnSignal();
void sendBrighterSignal();
void sendDarkerSignal();
void sendSignal();
int readSignalBuffer();

#endif
