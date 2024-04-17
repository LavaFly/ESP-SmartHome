#include <Arduino.h>
#include "sensor_handling.h"
#include "webserver_handling.h"
#include "time_handling.h"

String serialInput;

void sensorCallback();

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

// ota (https://docs.platformio.org/en/latest/platforms/espressif8266.html#over-the-air-ota-update)

void loop() {
    MDNS.update();

    setTimerSecondsCallback(10, &sensorCallback);
}

void sensorCallback(){
    if(!updateTimeClient()){
        Serial.println("failed to update timeClient");
    }
    if(!updateSensorValues()){
        Serial.println("failed to read out sensors");
    }

}
