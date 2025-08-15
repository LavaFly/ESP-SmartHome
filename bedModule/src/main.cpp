#include "ir_handling.h"
#include "webserver_handling.h"

void setup(){
    Serial.begin(9600);
    Serial.println("Starting setup");
    pinMode(13, INPUT);

    /**

    uint8_t hardwareInit = true;
    //hardwareInit = hardwareInit && buildIrConnection();

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
    **/

    Serial.println("Ending setup\n");
}

void loop(){
    /**
    MDNS.update();
    loopOTA();
    if(checkForNewMessage() != 0){
        sendSignal();
    }
    **/
    uint8_t touchResponse = digitalRead(13);
    Serial.print("Touch = ");
    Serial.println(touchResponse);
    delay(200);
}
