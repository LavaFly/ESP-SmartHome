#include "ir_handling.h"
#include "IRremoteInt.h"
#include <IRremote.hpp>


void buildIrConnection(){
    IrReceiver.begin(IR_RECEIVE_PIN, true);
}

uint8_t decodeIR(){
    if(IrReceiver.decode()){
        /**
        the following code is kept for debug purposes
        IrReceiver.printIRResultShort(&Serial);
        IrReceiver.printIRSendUsage(&Serial);
        if(IrReceiver.decodedIRData.protocol == UNKNOWN){
            Serial.println("he");
            IrReceiver.printIRResultRawFormatted(&Serial, true);
        }
        **/
        IrReceiver.resume();
        if(!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)){
            return IrReceiver.decodedIRData.command;
        }
    }
    return 0;
}
