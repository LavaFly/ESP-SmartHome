//#include <Arduino.h>
#include "ir_handling.h"
#include "webserver_handling.h"

void setup() {
    Serial.begin(9600);

    buildRouterConnection();
    buildIrConnection();
    initWebserver();
    setupMDNS();
}

void loop() {
    MDNS.update();
    if(readSignalBuffer() != 0){
        sendSignal();
    }
}
