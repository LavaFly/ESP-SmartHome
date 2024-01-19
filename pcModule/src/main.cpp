#include <Arduino.h>
#include "led_handling.h"
#include "webserver_handling.h"
#include "ir_handling.h"

#define STATUSPIN 0
#define POWERPIN 0

bool pcStatus = false;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");
    buildRouterConnection();
    initWebserver();
    setupMDNS();
    //buildTimeConnection();
}

void loop() {
    MDNS.update();
    if(Serial.available() > 0){
        serialInput = Serial.readStringUntil('\n');
        if(serialInput.equals("bg")){
            Serial.println("background");
        }
    }
}
