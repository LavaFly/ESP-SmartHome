#include <Arduino.h>
#include "webserver_handling.h"
#include "time_handling.h"

String serialInput;

void sensorCallback();

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");

    buildRouterConnection();
    buildTimeConnection();
    initWebserver();
    setupMDNS();

    Serial.println("setup done");
    Serial.println("OTA update");
}

// fancier website

void loop() {
    MDNS.update();
    loopOTA();
}
