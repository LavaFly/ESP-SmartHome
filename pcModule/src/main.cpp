#include <Arduino.h>
#include "sensor_handling.h"
#include "webserver_handling.h"

#define STATUSPIN 0
#define POWERPIN 5

bool pcStatus = false;
String serialInput;

unsigned long timeNow = 0;
unsigned long timeLast = 0;
uint8_t seconds = 0;

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("Starting...");
    pinMode(POWERPIN, OUTPUT);
    buildRouterConnection();
    initWebserver();
    setupMDNS();
    initSensor();
}

void loop() {
    timeNow = millis()/1000;
    seconds = timeNow - timeLast;
    if(seconds >= 5){
        timeLast = timeNow;
        updateSensorValues();
        updateTimeClient();
    }
    MDNS.update();
    loopOTA();

    /*
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
    */
}
