#ifndef IR_HANDLING
#define IR_HANDLING

#include <stdint.h>

#define IR_RECEIVE_PIN  5
#define DECODE_NEC

uint8_t buildIrConnection();
uint8_t decodeIR();

#endif
