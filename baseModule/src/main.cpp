#include <Arduino.h>
#include "led_handling.h"
#include "webserver_handling.h"
#include "ir_handling.h"

String serialInput;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");
    buildIrConnection();
    buildRouterConnection();
    buildTimeConnection();
    //buildLedConnection();
    //initialiseLedMap();
    //clearActiveLeds();
    setupMDNS();
}

void loop() {
    MDNS.update();
    uint8_t irInput = decodeIR();
    // will later be replaced by some proper mapping of each button to a
    // function, but havent decided most of them yet so this will suffice
    switch (irInput) {
        case 0x12:
            Serial.println("lighting on");
            httpGetRequestIgnoreResponse("http://lightingModule.local/lightingOn");
            break;
        case 0x1e:
            Serial.println("lighting off");
            httpGetRequestIgnoreResponse("http://lightingModule.local/lightingOff");
            break;
        case 0x01:
            Serial.println("background");
            break;
        case 0x03:
            Serial.println("projecting time");
            /*
            struct simpleTime * currentTime = (struct simpleTime*)malloc(sizeof(struct simpleTime));
            getSimpleTime(currentTime);
            projectTime(currentTime->hour, currentTime->minute);
            free(currentTime);
            */
            break;
        case 0x0e:
            Serial.println("raising brightness");
            httpGetRequestIgnoreResponse("http://lightingModule.local/raiseBrightness");
            break;
        case 0x0c:
            Serial.println("lowering brightness");
            httpGetRequestIgnoreResponse("http://lightingModule.local/lowerBrightness");
            break;
        case 0x1a:
            httpGetRequestIgnoreResponse("http://pcModule.local/pcPowerOn");
            break;
    }

    if(false){//Serial.available() > 0
        clearActiveLeds();
        serialInput = Serial.readStringUntil('\n');
        if(serialInput.equals("bg")){
            // toggle background
            Serial.println("background");
            backgroundEvent();
        } else if (serialInput.equals("ra")) {
            // print example string message
            Serial.println("Example String");
            projectExampleString();
        } else {
            Serial.println("got number");
            uint16_t serialNumber = serialInput.toInt() % 65535;
            if(serialNumber) {
                Serial.printf("got number %d", serialNumber);
                projectNumber(serialNumber);

            }
        }
    }
}
/**
void loop() {
    MDNS.update();

    if(false){
        uint16_t irData = decodeIR();
        if(Serial.available() > 0){
            clearActiveLeds();
            serialInput = Serial.readStringUntil('\n');
            if(serialInput.equals("bg")){
                // toggle background
                Serial.println("background");
            } else if (serialInput.equals("ra")) {
                // print example string message
                Serial.println("Example String");
                projectExampleString();
            } else {
                Serial.println("projecting time");
                struct simpleTime * currentTime = (struct simpleTime*)malloc(sizeof(struct simpleTime));
                getSimpleTime(currentTime);
                projectTime(currentTime->hour, currentTime->minute);
                free(currentTime);
            }
        }
    }
}
*/
