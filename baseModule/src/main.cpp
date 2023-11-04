#include <Arduino.h>
#include "led_handling.h"

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");

    // initialise connection to the led-wall
    buildLedConnection();
    initialiseLedMap();
    clearActiveLeds();
    projectTime(12, 34);
    delay(4000);
    clearActiveLeds();
}

void loop() {
    if(false){
    //if(Serial.available() > 0){
        String input = Serial.readStringUntil('\n');
        Serial.println("showing time");

        projectTime(12, 34);
        delay(4000);
        clearActiveLeds();
    }
}
