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


bool loopActive = false;
bool animationActive = false;
bool toBeCleared = false;

void setup() {
    //Serial.begin(9600,SERIAL_8N1,SERIAL_TX_ONLY); // to limit inbound serial comminucation from interefering
                                                  // with the ir_handling
    Serial.begin(9600);
    Serial.println("starting the setup");

    buildIrConnection();
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
    // remove this serial part as it is only for debugging
    if(Serial.available() > 0){
        clearActiveLeds();
        serialInput = Serial.readStringUntil('\n');
        if(serialInput.equals("bg")){
            // toggle background
            Serial.println("background");
            backgroundEvent();
        } else if (serialInput.equals("e")) {
            // print example string message
            const char* exampleString = "Hello World";

            uint8_t num = strlen(exampleString);
            startSlideAnimation(exampleString, num);
            animationActive = true;

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
            char* weatherDescription = (char*)malloc(25 * sizeof(char));
            getWeatherDescription(weatherDescription);

            uint8_t num = strlen(weatherDescription);
            Serial.println(num);
            Serial.print("weather data = ");
            Serial.println(weatherDescription);
            startSlideAnimation(weatherDescription, num);

            animationActive = true;
            free(weatherDescription);

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


    irInput = decodeIR();
    // will later be replaced by some proper mapping of each button to a
    // function, but havent decided most of them yet so this will suffice

    struct simpleTime * currentTimeStruct;
    switch (irInput) {
        case 0x12:
            Serial.println("lighting on");
            httpGetRequestIgnoreResponse("http://lightingModule.local/lightingOn");
            break;
        case 0x1e:
            Serial.println("lighting off");
            httpGetRequestIgnoreResponse("http://lightingModule.local/lightingOff");
            break;
        case 0x0e:
            httpGetRequestIgnoreResponse("http://lightingModule.local/raiseBrightness");
            break;
        case 0x0c:
            httpGetRequestIgnoreResponse("http://lightingModule.local/lowerBrightness");
            break;
        case 0x01:
            Serial.println("Example String");

            Serial.println("WeatherData");
            char* weatherDescription = (char*)malloc(25 * sizeof(char));
            getWeatherDescription(weatherDescription);

            uint8_t num = strlen(weatherDescription);
            Serial.println(num);
            Serial.print("weather data = ");
            Serial.println(weatherDescription);
            startSlideAnimation(weatherDescription, num);

            animationActive = true;
            free(weatherDescription);

            Serial.println("done");
            break;
            /**
        case 0x03:
            currentTimeStruct = (struct simpleTime*)malloc(sizeof(struct simpleTime));
            Serial.println("projecting time");
            getSimpleTime(currentTimeStruct);
            projectTime(currentTimeStruct->hour, currentTimeStruct->minute);
            free(currentTimeStruct);
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
        **/
    }
}

void showTime(){
    struct simpleTime * currentTimeStruct;
    currentTimeStruct = (struct simpleTime*)malloc(sizeof(struct simpleTime));
    Serial.println("projecting time");

    getSimpleTime(currentTimeStruct);
    Serial.print(currentTimeStruct->hour);
    Serial.print(":");
    Serial.println(currentTimeStruct->minute);

    projectTime(currentTimeStruct->hour, currentTimeStruct->minute);

    clearTimer = millis();
    toBeCleared = true;

    free(currentTimeStruct);
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
