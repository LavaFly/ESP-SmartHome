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
}

void loop() {

}
