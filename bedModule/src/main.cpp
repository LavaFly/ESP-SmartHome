#include "touch_handling.h"
#include "webserver_handling.h"

void setup(){
    Serial.begin(9600);
    Serial.println("Starting setup");
    pinMode(BUTTON_PIN, INPUT);
    // there is no real hardware init, as the button is just a digitalRead

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

extern void newLoop();

void loop(){
    MDNS.update();
    loopOTA();
    handleButtonInput();
    updateStateMachine();
}
