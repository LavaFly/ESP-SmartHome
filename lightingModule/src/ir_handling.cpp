#include "ir_handling.h"
#include <IRremote.hpp>


void buildIrConnection(){
    IrSender.begin(false);
}

void sendOffSignal(){
    IrSender.sendNEC(3, 0x80, 0x1e);
}
void sendOnSignal(){
    IrSender.sendNEC(3, 0x80, 0x12);
};
void sendHigherSignal(){
    IrSender.sendNEC(3, 0x80, 0x0e);
}
void sendLowerSignal(){
    IrSender.sendNEC(3, 0x80, 0x0c);
}

