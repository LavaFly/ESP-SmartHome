#include "ir_handling.h"
#include "IRremoteInt.h"
#include <IRremote.hpp>


uint8_t buildIrConnection(){
    // check this at some point
    IrReceiver.begin(IR_RECEIVE_PIN, true);
    return 1;
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
