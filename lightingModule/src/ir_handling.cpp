#include "ir_handling.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define IR_PIN 4

IRsend IrSender(IR_PIN);
uint8_t signalContent = 0;
uint32_t signalData;
uint8_t lightIsOn;
uint8_t reset;

uint32_t resetData;
uint8_t currentBrightness;

uint8_t buildIrConnection(){
    // check if this can fail
    IrSender.begin();
    lightIsOn = false;

    // do this once here
    resetData = IrSender.encodeNEC(0x80, LIGHT_DARKER);
    return 1;
}

// the async webserver doesnt like it, when the response functions do too much stuff
// so im only setting storing the desired signal upon a webserver request and sending
// them later in the main loop
void sendOffSignal(){
    signalContent = LIGHT_OFF;
    lightIsOn = false;
}

void sendOnSignal(){
    signalContent = LIGHT_ON;
    lightIsOn = true;
}

void sendBrighterSignal(){
    signalContent = LIGHT_BRIGHTER;
}

void sendDarkerSignal(){
    signalContent = LIGHT_DARKER;
}

void sendSignal(){
    signalData = IrSender.encodeNEC(0x80, signalContent);
    if(signalContent == LIGHT_BRIGHTER){
        for(uint8_t i = 0; i < currentBrightness - 1; i++){
            IrSender.sendNEC(signalData, 32, 2);
            delay(100);
        }
    } else {
        IrSender.sendNEC(signalData, 32, 2);
    }
    signalContent = 0;
}

// if return != 0 then new message
uint8_t checkForNewMessage(){
    return signalContent;
}

void toggleLight(){
    if(lightIsOn){
        sendOffSignal();
    } else {
        sendOnSignal();
    }
}

uint8_t checkReset(){
    return reset;
}

void resetBrightness(){
    reset = true;
}

void resetTheBrightness(){
    IrSender.sendNEC(resetData, 32, 2);
    delay(100);
    IrSender.sendNEC(resetData, 32, 2);
    delay(100);
    IrSender.sendNEC(resetData, 32, 2);
    delay(100);
    reset = false;
}

void setBrightness(uint8_t brightnessLevel){
    resetBrightness();
    currentBrightness = brightnessLevel;
    signalContent = LIGHT_BRIGHTER;
}
