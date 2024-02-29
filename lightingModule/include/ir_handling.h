#ifndef IR_HANDLING
#define IR_HANDLING

#define IR_SEND_PIN  12

void buildIrConnection();
void sendOffSignal();
void sendOnSignal();
void sendHigherSignal();
void sendLowerSignal();

#endif
