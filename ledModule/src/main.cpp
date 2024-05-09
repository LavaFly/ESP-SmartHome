#include <Arduino.h>
#include <cstdint>
#include "led_handling.h"
#include "webserver_handling.h"
#include "time_handling.h"
#include "ir_handling.h"
#include "weather_handling.h"
#include "voice_handling.h"

String serialInput;
uint8_t irInput;

// remove this later with proper implementation in time_handling

unsigned long currentTime = 0;
unsigned long clearTimer = 0;
unsigned long vrClearTimer = 0;


bool loopActive = false;
bool animationActive = false;
bool toBeCleared = false;


// 1 = PC, 2 = Light
uint8_t vrTreeBranch = 0;
void setupEventResponse();

void vr_pc();
void vr_light();
void vr_time();
void vr_weather();
void vr_temperature();
void vr_co2();
void vr_on();
void vr_off();
void vr_brighter();
void vr_darker();
void vr_empty();

void setup() {
    //Serial.begin(9600,SERIAL_8N1,SERIAL_TX_ONLY); // to limit inbound serial comminucation from interefering
                                                  // with the ir_handling
    Serial.begin(9600);
    Serial.println("starting the setup");

    /**
    buildIrConnection();
    **/
    initVR();
    buildRouterConnection();
    buildTimeConnection();
    initWebserver();
    setupMDNS();

    // setup for the ledWall
    buildLedConnection();
    initialiseLedMap();
    clearActiveLeds();

    setupEventResponse();

    currentTime = millis();
    Serial.println("finishing the setup");
}

void loop() {
    MDNS.update();
    loopOTA();
    handleVR();

    cleanUpSockets();
    if(animationActive && millis() > currentTime + 150){
        if(!advanceSlideAnimation()){
            animationActive = false;
            if(loopActive){
                Serial.println("restarting");
                uint8_t num = serialInput.length();
                startSlideAnimation(serialInput.c_str(), num);
                animationActive = true;
            }
        }
        currentTime = millis();
    }
    if(toBeCleared && millis() > clearTimer + 5000){
        clearActiveLeds();
        clearActiveLeds();
        toBeCleared = false;
        Serial.println("clearing Screen");
    }

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
            const char* exampleString = "AEio123 o o";

            uint8_t num = 12;
            startSlideAnimation(exampleString, num);

            Serial.println("done");
        } else if (serialInput.equals("loop")) {
            Serial.println("loop on");

            serialInput = "Hallo";

            uint8_t num = 5;
            startSlideAnimation(serialInput.c_str(), num);
            loopActive = !loopActive;
            animationActive = true;

            Serial.println("done");
        } else if (serialInput.equals("we")){
            Serial.println("WeatherData");
            char* weatherDescription = (char*)malloc(20 * sizeof(char));
            getWeatherDescription(weatherDescription);

            uint8_t num = strlen(weatherDescription);
            Serial.println(num);
            Serial.println(weatherDescription);
            startSlideAnimation(weatherDescription, num);
            animationActive = true;

            Serial.println("done");
        } else if(serialInput.equals("m")){
            mothersDayMessage();
            animationActive = true;
        } else {

            Serial.print("Printing: ");
            Serial.println(serialInput);

            uint8_t num = serialInput.length();
            startSlideAnimation(serialInput.c_str(), num);
            animationActive = true;

        }
    }

    /**

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
    **/
}

void showTime(){
    struct simpleTime * currentTime;
    currentTime = (struct simpleTime*)malloc(sizeof(struct simpleTime));
    Serial.println("projecting time");

    getSimpleTime(currentTime);
    Serial.print(currentTime->hour);
    Serial.print(":");
    Serial.println(currentTime->minute);

    projectTime(currentTime->hour, currentTime->minute);

    clearTimer = millis();
    toBeCleared = true;

    free(currentTime);
}

void setupTextAnimation(String message){
    Serial.println("starting animation");
    Serial.print("message: ");
    Serial.println(message);
    serialInput = message;
    uint8_t num = serialInput.length();
    startSlideAnimation(serialInput.c_str(), num);
    animationActive = true;
}

void setupEventResponse(){
    addToEventReponse(0, vr_pc);
    addToEventReponse(1, vr_light);
    addToEventReponse(2, vr_time);
    addToEventReponse(3, vr_weather);
    addToEventReponse(4, vr_temperature);
    addToEventReponse(5, vr_co2);
    addToEventReponse(6, vr_empty);
    addToEventReponse(7, vr_empty);
    addToEventReponse(8, vr_on);
    addToEventReponse(9, vr_off);
    addToEventReponse(10, vr_brighter);
    addToEventReponse(11, vr_darker);
}

void vr_pc(){
    vrTreeBranch = 1;
    loadOnOff();
}
void vr_light(){
    vrTreeBranch = 2;
    loadOnOffBrighterDarker();
}
void vr_time(){
    Serial.println("");
    showTime();
}
void vr_weather(){
    Serial.println("");

}
void vr_temperature(){
    Serial.println("");

}
void vr_co2(){
    Serial.println("");

}
void vr_on(){
    Serial.println("");
    if(vrTreeBranch == 1){
        httpGetRequestIgnoreResponse("http://lightingModule.local/pcPowerOn");
    } else if(vrTreeBranch == 2){
        httpGetRequestIgnoreResponse("http://lightingModule.local/lightingOn");
    } else {

    }
    loadDefaultVR();
}
void vr_off(){
    Serial.println("");
    if(vrTreeBranch == 1){
        Serial.println("not properly implemented, as i havent spliced the necessary cable yet");
        httpGetRequestIgnoreResponse("http://lightingModule.local/pcPowerOn");
    } else if(vrTreeBranch == 2){
        httpGetRequestIgnoreResponse("http://lightingModule.local/lightingOff");
    } else {

    }
    loadDefaultVR();
}
void vr_brighter(){
    if(vrTreeBranch == 1){
        Serial.println("impossible");
    } else if(vrTreeBranch == 2){
        httpGetRequestIgnoreResponse("http://lightingModule.local/raiseBrightness");
    } else {

    }
    loadDefaultVR();
}
void vr_darker(){
    if(vrTreeBranch == 1){
        Serial.println("impossible");
    } else if(vrTreeBranch == 2){
        httpGetRequestIgnoreResponse("http://lightingModule.local/lowerBrightness");
    } else {

    }
    loadDefaultVR();
}
void vr_empty(){
    loadDefaultVR();
}
