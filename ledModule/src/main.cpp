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
bool endlessAnimation = false;

timerElement* mainAnimationTimer;
timerElement* mainScreenClearTimer;

void showTime();
void showExample();


void setup() {
    //Serial.begin(9600,SERIAL_8N1,SERIAL_TX_ONLY); // to limit inbound serial comminucation from interefering
                                                  // with the ir_handling
    Serial.begin(9600);
    Serial.println("Starting setup");



    uint8_t hardwareInit = true;
    hardwareInit = hardwareInit && buildIrConnection();
    //hardwareInit = hardwareInit && initVR();

    if(hardwareInit){
        Serial.println("Hardware init successful");
    } else {
        Serial.println("Hardware init failed");
    }

    uint8_t softwareInit = true;
    softwareInit = softwareInit && buildRouterConnection();
    softwareInit = softwareInit && buildTimeConnection();
    softwareInit = softwareInit && initWebserver();
    softwareInit = softwareInit && setupMDNS();

    buildLedConnection();
    initialiseLedMap();
    clearActiveLeds();

    setupEventMap();

    if(softwareInit){
        Serial.println("Software init successful");
    } else {
        Serial.println("Software init failed");
    }

    Serial.println("Ending setup\n");
}

void loop() {
    MDNS.update();
    loopOTA();
    //handleVR();
    cleanUpSockets();


    if(animationActive){
        if(checkTimer(mainAnimationTimer)){
            resetTimer(mainAnimationTimer);
            if(!advanceSlideAnimation()){
                // animation is done
                animationActive = false;
                deleteTimer(mainAnimationTimer);
            }
        }
    }

    if(checkTimer(mainScreenClearTimer)){
        Serial.println("mainScreenClearTimer has run out");
        clearActiveLeds();
        deleteTimer(mainScreenClearTimer);
        mainScreenClearTimer = NULL;
    }

    irInput = decodeIR();
    // will later be replaced by some proper mapping of each button to a
    // function, but havent decided most of them yet so this will suffice

    switch (irInput) {
        // codes corresponding to the buttons on my remote
        case 0x12: // on
            Serial.println("lighting on");
            httpGetRequestIgnoreResponse("http://lighting.local/on");
            break;
        case 0x1e: // off
            Serial.println("lighting off");
            httpGetRequestIgnoreResponse("http://lighting.local/off");
            break;
        case 0x0e: // +
            httpGetRequestIgnoreResponse("http://lighting.local/raiseBrightness");
            break;
        case 0x0c: // -
            httpGetRequestIgnoreResponse("http://lighting.local/lowerBrightness");
            break;
        case 0x03: // 2
            showTime();
            break;
        case 0x1a: // timer
            httpGetRequestIgnoreResponse("http://pc.local/on");
            break;
        case 0x01: // 1
            showExample();
            break;
        case 0x04: // 3
            handleTemperatureRequest(NULL);
        case 0x06: // 4
            handleCO2Request(NULL);
        case 0x07: // 5

        case 0x09: // 6
        case 0x0a: // 7
        case 0x1f: // 8
        default:
            /**
             * the current setup is connected on a local network
             * but has no internet access(one mod runs a ntp setver, which
             * is how i get the time), this means i cant get the weather data
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
            **/
            break;
    }

}

void showTime(){
    struct simpleTime * currentTimeStruct;
    currentTimeStruct = (struct simpleTime*)malloc(sizeof(struct simpleTime));
    Serial.println("projecting time");

    getSimpleTime(currentTimeStruct);
    projectTime(currentTimeStruct->hour, currentTimeStruct->minute);

    mainScreenClearTimer = addTimer(3);

    free(currentTimeStruct);
}
void showExample(){
    const char* example = "Hello World";

    uint8_t num = strlen(example);
    startSlideAnimation(example, num);
    animationActive = true;
    mainAnimationTimer = addTimerMilliseconds(200);
}

void setupTextAnimation(String message){
    Serial.println("starting animation");
    Serial.print("message: ");
    Serial.println(message);
    serialInput = message;
    uint8_t num = serialInput.length();
    startSlideAnimation(serialInput.c_str(), num);
    animationActive = true;
    mainAnimationTimer = addTimerMilliseconds(200);
}
