#ifndef IR_HANDLING
#define IR_HANDLING

#include <stdint.h>

#define IR_RECEIVE_PIN  14
#define DECODE_NEC

void buildIrConnection();
uint16_t decodeIR();

#endif
