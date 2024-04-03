#include "ir_handling.h"
#include <IRremote.hpp>


void buildIrConnection(){
    IrReceiver.begin(IR_RECEIVE_PIN, true);
}

uint16_t decodeIR(){
    if(IrReceiver.decode()){
        IrReceiver.printIRResultShort(&Serial);
        IrReceiver.printIRSendUsage(&Serial);
        if(IrReceiver.decodedIRData.protocol == UNKNOWN){
            Serial.println("he");
            IrReceiver.printIRResultRawFormatted(&Serial, true);
        }
        Serial.println();
        IrReceiver.resume();
        return IrReceiver.decodedIRData.command;
    }
    return -1;
}
