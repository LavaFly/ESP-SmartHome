#include "ir_handling.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>

IRsend IrSender(4);
uint8_t signalContent = 0;
uint32_t signalData;

uint8_t buildIrConnection(){
    // check if this can fail
    IrSender.begin();
    return 1;
}

// the async webserver doesnt like it, when the response functions do too much stuff
// so im only setting storing the desired signal upon a webserver request and sending
// them later in the main loop
void sendOffSignal(){
    signalContent = LIGHT_OFF;
}
void sendOnSignal(){
    signalContent = LIGHT_ON;
}
void sendBrighterSignal(){
    signalContent = LIGHT_BRIGHTER;
}
void sendDarkerSignal(){
    signalContent = LIGHT_DARKER;
}
void sendSignal(){
    signalData = IrSender.encodeNEC(0x80, signalContent);
    IrSender.sendNEC(signalData, 32, 2);
    signalContent = 0;
}

// if return != 0 then new message
uint8_t checkForNewMessage(){
    return signalContent;
}

