#include "touch_handling.h"
#include "webserver_handling.h"

void setup(){
    Serial.begin(9600);
    Serial.println("Starting setup");
    pinMode(BUTTON_PIN1, INPUT);
    pinMode(BUTTON_PIN2, INPUT);
    // there is no real hardware init, as the button is just a digitalRead

    uint8_t softwareInit = true;
    softwareInit = softwareInit && buildRouterConnection();
    softwareInit = softwareInit && initWebserver();
    softwareInit = softwareInit && setupMDNS();

    softwareInit = softwareInit && setupButtons();

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
    touchLoop();
}
