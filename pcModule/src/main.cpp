#include <Arduino.h>
#include "sensor_handling.h"
#include "webserver_handling.h"
#include "time_handling.h"


bool pcStatus = false;

String serialInput;

void sensorCallback();

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("Starting...");
    buildRouterConnection();
    initWebserver();
    setupMDNS();
    initSensor();
    Serial.println("setup done");
}

void loop() {
    MDNS.update();
    loopOTA();

    setTimerMinutesCallback(10, &sensorCallback);
}

void sensorCallback(){
    Serial.println(getEpochTime());
    if(!updateTimeClient()){
        Serial.println("failed to update timeClient");
    }
    if(!updateSensorValues()){
        Serial.println("failed to read out sensor");
    }
}
