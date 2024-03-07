#include "ir_handling.h"

int signalBuffer = 0;

void buildIrConnection(){
    //IrSender.begin(false);
}

void sendOffSignal(){
    signalBuffer = LIGHT_OFF;
}
void sendOnSignal(){
    signalBuffer = LIGHT_ON;
}
void sendBrighterSignal(){
    signalBuffer = LIGHT_BRIGHTER;
}
void sendDarkerSignal(){
    signalBuffer = LIGHT_DARKER;
}
void sendSignal(){
    //IrSender.sendNEC(2, IR_ADDRESS, signalBuffer);
    Serial.print(signalBuffer);
    signalBuffer = 0;
}

int readSignalBuffer(){
    return signalBuffer;
}

