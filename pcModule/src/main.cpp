#include <Arduino.h>
#include "webserver_handling.h"

#define STATUSPIN 0
#define POWERPIN 14

bool pcStatus = false;
String serialInput;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");
    pinMode(POWERPIN, OUTPUT);
    buildRouterConnection();
    initWebserver();
    setupMDNS();
}

void loop() {
    MDNS.update();
    if(Serial.available() > 0){
        serialInput = Serial.readStringUntil('\n');
        if(serialInput.equals("on")){
            Serial.println("power on");
            digitalWrite(POWERPIN, HIGH);
        } else if (serialInput.equals("off")) {
            Serial.println("power off");
            digitalWrite(POWERPIN, LOW);
        }
    }
}
