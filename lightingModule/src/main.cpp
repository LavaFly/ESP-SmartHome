#include <Arduino.h>
#include "webserver_handling.h"

uint8_t lightingBrightness;
bool lightingStatus;

String serialInput;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");


    buildRouterConnection();
    initWebserver();
}

void loop() {

}
