#include <Arduino.h>
#include "led_handling.h"

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");

    // initialise connection to the led-wall
    buildLedConnection();
    initialiseLedMap();

    clearActiveLeds();
    projectDigit(4, 3, 1);
    projectDigit(2, 8, 1);
    projectDigit(0, 17, 1);
    delay(4000);
    clearActiveLeds();
    projectNumber(4203);
    delay(4000);
    clearActiveLeds();
    projectNumber(420);
    delay(4000);
    clearActiveLeds();
    projectNumber(42);
    delay(4000);
    clearActiveLeds();
    projectNumber(4);
    delay(4000);
    clearActiveLeds();
}

void loop() {

    if(false){
    //if(Serial.available() > 0){
        Serial.readStringUntil('\n');
        Serial.println("got input");

        projectDigit(5, 6, 1);
        projectDigit(3, 12, 1);
        //projectTest();
        Serial.println("got printing");
        delay(5000);
        clearActiveLeds();
        Serial.println("claeed");

        //Serial.println("showing time again");
        projectNumber(456);
        //projectTime(hour + 6, minute + 4);
        //delay(5000);
        //clearActiveLeds();
    }
}
