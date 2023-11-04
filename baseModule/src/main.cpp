#include <Arduino.h>
#include "led_handling.h"
#include "webserver_handling.h"

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");

    buildRouterConnection();
    buildTimeConnection();
    // initialise connection to the led-wall
    buildLedConnection();
    initialiseLedMap();
    clearActiveLeds();
}

void loop() {
    if(Serial.available() > 0){
        int byte = Serial.read();

        Serial.println("Got content");
        struct simpleTime * currentTime = (struct simpleTime*)malloc(sizeof(struct simpleTime));
        getSimpleTime(currentTime);
        projectTime(currentTime->hour, currentTime->minute);
        free(currentTime);
    }
}
