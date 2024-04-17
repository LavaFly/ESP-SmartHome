#include <Arduino.h>
#include "sensor_handling.h"
#include "webserver_handling.h"

String serialInput;

unsigned long timeNow = 0;
unsigned long timeLast = 0;
uint8_t seconds = 0;
uint8_t minutes = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");

    initSensor();
    buildRouterConnection();
    buildTimeConnection();
    initWebserver();
    setupMDNS();
    Serial.println("setup done");
}

// return sensorReadings as json
// ota (https://docs.platformio.org/en/latest/platforms/espressif8266.html#over-the-air-ota-update)

void loop() {
    MDNS.update();
    timeNow = millis()/1000;
    seconds = timeNow - timeLast;
    if(seconds >= 60){
        timeLast = timeNow;
        minutes++;
    }
    if(minutes >= 10){
        //updateTimeClient();
        updateSensorValues();
        minutes = 0;
    }
}
