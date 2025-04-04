#include <Arduino.h>
#include "sensor_handling.h"
#include "actuator_handling.h"
#include "webserver_handling.h"
#include "time_handling.h"

void sensorCallback();

uint8_t pumpWorkaround = 0;
uint8_t pumpActive = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");

    initSensor();
    initActuator();

    buildRouterConnection();
    buildTimeConnection();
    initWebserver();
    setupMDNS();
    Serial.println("setup done");
}

void loop() {
    MDNS.update();
    loopOTA();

    /**
    if(pumpActive){
        setTimerSecondsCallback(pumpWorkaround, &deactivateActuator);
    }

    setTimerSecondsCallback(10, &sensorCallback);

    for(uint8_t daysCounter = 0; daysCounter < 2; daysCounter++){
        for(uint8_t hoursCounter = 0; hoursCounter < 24; hoursCounter++){
            for(uint8_t minutesCounter = 0; minutesCounter < 60; minutesCounter++){
                delay(60000);
            }
        }
    }

    //pump 400ml water
    digitalWrite(PUMP_POWER, HIGH);
    delay(20000);
    digitalWrite(PUMP_POWER, LOW);
    **/
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
