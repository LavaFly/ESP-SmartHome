// this requires the IRremote Library

#include <Arduino.h>

#include "PinDefinitionsAndMore.h"

#include <IRremote.hpp>

void setup(){
    Serial.begin(9600);
    IrSender.begin();
}

void loop(){
    while(Serial.available() == 0){}
    int receivedCommand = Serial.parseInt();
    // 0x80 is the adress of the lighting
    IrSender.sendNEC(0x80, receivedCommand, 3);
    // delay to prevent accidental spamming
    delay(100);
}
