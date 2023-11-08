#include <Arduino.h>
#include "webserver_handling.h"

String serialInput;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");

    buildRouterConnection();
}

void loop() {

}
