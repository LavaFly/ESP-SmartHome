#include "ir_handling.h"
#include <IRremote.hpp>


void buildIrConnection(){
    IrSender.begin(false);
}

void sendOffSignal(){
    signalBuffer = LIGHT_OFF;
}
void sendOnSignal(){
    signalBuffer = LIGHT_ON;
}
void sendBrigherSignal(){
    signalBuffer = LIGHT_BRIGHTER;
}
void sendDarkerSignal(){
    signalBuffer = LIGHT_DARKER;
}
void sendSignal(){
    IrSender.sendNEC(2, IR_ADDRESS, signalBuffer);
    signalBuffer = 0;
}

