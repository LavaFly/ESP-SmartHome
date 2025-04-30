#include <Arduino.h>
#include "sd_handling.h"
#include "sensor_handling.h"
#include "webserver_handling.h"
#include "time_handling.h"


void sensorCallback();


void setup() {
    Serial.begin(9600);
    Serial.println("Starting setup");
    uint8_t hardwareInit = true;
    hardwareInit = hardwareInit && initSensor();
    //hardwareInit = hardwareInit && initSDCard();
    initSDCard();

    if(hardwareInit){
        Serial.println("Hardware init successful");
    } else {
        Serial.println("Hardware init failed");
    }

    uint8_t softwareInit = true;
    softwareInit = softwareInit && buildRouterConnection();
    softwareInit = softwareInit && buildTimeConnection();
    softwareInit = softwareInit && initWebserver();
    softwareInit = softwareInit && setupMDNS();

    if(softwareInit){
        Serial.println("Software init successful");
    } else {
        Serial.println("Software init failed");
    }

    Serial.println("Ending setup\n");
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
        Serial.println("failed to read out sensors");
    }
}
