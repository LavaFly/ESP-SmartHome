#include <Arduino.h>
#include <cstdint>
#include "led_handling.h"
#include "webserver_handling.h"
#include "time_handling.h"
#include "ir_handling.h"

String serialInput;
uint8_t irInput;

// remove this later with proper implementation in time_handling

unsigned long currentTime = 0;
unsigned long lastTime = 0;
uint8_t currentSeconds = 0;


void setup() {
    //Serial.begin(9600,SERIAL_8N1,SERIAL_TX_ONLY); // to limit inbound serial comminucation from interefering
                                                  // with the ir_handling
    Serial.begin(9600);
    Serial.println("Starting...");
    buildIrConnection();
    buildRouterConnection();
    buildTimeConnection();

    // setup for the ledWall
    buildLedConnection();
    initialiseLedMap();
    clearActiveLeds();
}

void loop() {
    if(Serial.available() > 0){
        clearActiveLeds();
        serialInput = Serial.readStringUntil('\n');
        if(serialInput.equals("bg")){
            // toggle background
            Serial.println("background");
            backgroundEvent();
        } else if (serialInput.equals("ex")) {
            // print example string message
            Serial.println("Example String");

            for(int i = 25; i > -46; i--){
                projectExampleString(i);
                delay(180);
                //Serial.println(i);
            }
            Serial.println("done");
        } else {
            /**
            Serial.println("got number");
            uint16_t serialNumber = serialInput.toInt() % 65535;
            if(serialNumber) {
                Serial.printf("got number %d", serialNumber);
                //projectNumber(serialNumber);
                projectExampleString((uint8_t)serialNumber);

            }
            **/
        }
    }


    irInput = decodeIR();
    // will later be replaced by some proper mapping of each button to a
    // function, but havent decided most of them yet so this will suffice

    struct simpleTime * currentTime;
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
            Serial.println("Example String");

            for(int i = 25; i > -46; i--){
                projectExampleString(i);
                delay(200);
                //Serial.println(i);
            }
            break;
        case 0x03:
            currentTime = (struct simpleTime*)malloc(sizeof(struct simpleTime));
            Serial.println("projecting time");
            getSimpleTime(currentTime);
            projectTime(currentTime->hour, currentTime->minute);
            free(currentTime);
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
}
