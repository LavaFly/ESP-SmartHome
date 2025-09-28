#include "ir_handling.h"
#include "webserver_handling.h"

void setup(){
    Serial.begin(9600);
    Serial.println("Starting setup");
    uint8_t hardwareInit = true;
    hardwareInit = hardwareInit && buildIrConnection();

    if(hardwareInit){
        Serial.println("Hardware init successful");
    } else {
        Serial.println("Hardware init failed");
    }

    uint8_t softwareInit = true;
    softwareInit = softwareInit && buildRouterConnection();
    softwareInit = softwareInit && initWebserver();
    softwareInit = softwareInit && setupMDNS();

    if(softwareInit){
        Serial.println("Software init successful");
    } else {
        Serial.println("Software init failed");
    }

    Serial.println("Ending setup\n");
}

void loop(){
    MDNS.update();
    loopOTA();
    if(checkReset()){
        resetTheBrightness();
    }

    if(checkForNewMessage() != 0){
        sendSignal();
    }
}
