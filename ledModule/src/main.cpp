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
timerElement* animationTimer;
timerElement* screenClearTimer;

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


    buildLedConnection();
    initialiseLedMap();
    clearActiveLeds();

    setupEventMap();

    Serial.println("finishing the setup");
}

void loop() {
    MDNS.update();
    loopOTA();
    handleVR();
    cleanUpSockets();

    if(!checkTimer(animationTimer) || endlessAnimation){
        if(!advanceSlideAnimation()){
            // animation is done
            animationActive = false;
        }
        Serial.println("animating");
    }
    if(checkTimer(screenClearTimer)){
        clearActiveLeds();
        deleteTimer(screenClearTimer);
        screenClearTimer = NULL;
        Serial.println("cleared screen");
    }

    /**
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

    irInput = decodeIR();
    // will later be replaced by some proper mapping of each button to a
    // function, but havent decided most of them yet so this will suffice

    struct simpleTime * currentTimeStruct;
    switch (irInput) {
        case 0x12:
            Serial.println("lighting on");
            httpGetRequestIgnoreResponse("http://lighting.local/on");
            break;
        case 0x1e:
            Serial.println("lighting off");
            httpGetRequestIgnoreResponse("http://lighting.local/off");
            break;
        case 0x0e:
            httpGetRequestIgnoreResponse("http://lighting.local/raiseBrightness");
            break;
        case 0x0c:
            httpGetRequestIgnoreResponse("http://lighting.local/lowerBrightness");
            break;
        case 0x03:
            currentTimeStruct = (struct simpleTime*)malloc(sizeof(struct simpleTime));
            Serial.println("projecting time");
            getSimpleTime(currentTimeStruct);
            projectTime(currentTimeStruct->hour, currentTimeStruct->minute);
            screenClearTimer = addTimer(5);
            free(currentTimeStruct);
            break;
        case 0x1a:
            httpGetRequestIgnoreResponse("http://pcModule.local/pcPowerOn");
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
