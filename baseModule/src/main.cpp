#include <Arduino.h>
#include "led_handling.h"
#include "webserver_handling.h"
#include "ir_handling.h"

String serialInput;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");
    buildRouterConnection();
    buildTimeConnection();
    buildLedConnection();
    initialiseLedMap();
    clearActiveLeds();
}

void loop() {
    if(Serial.available() > 0){
        clearActiveLeds();
        serialInput = Serial.readStringUntil('\n');
        if(serialInput.equals("bg")){
            // toggle background
            Serial.println("background");
            backgroundEvent();
        } else if (serialInput.equals("ra")) {
            // print example string message
            Serial.println("Example String");
            projectExampleString();
        } else {
            Serial.println("got number");
            uint16_t serialNumber = serialInput.toInt() % 65535;
            if(serialNumber) {
                Serial.printf("got number %d", serialNumber);
                projectNumber(serialNumber);
            } else {
                Serial.println("projecting time");
                struct simpleTime * currentTime = (struct simpleTime*)malloc(sizeof(struct simpleTime));
                getSimpleTime(currentTime);
                projectTime(currentTime->hour, currentTime->minute);
                free(currentTime);
            }
        }
    }
}
