#include <ESP8266WiFi.h>
#include "wl_definitions.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "time_handling.h"

unsigned long timeLast = 0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "baseModule.local", 3600, 120000);//europe.pool.ntp.org


bool buildTimeConnection(){
    if(WiFi.status() == WL_CONNECTED){
        timeClient.begin();
        timeClient.setTimeOffset(7200);
        timeClient.update();
        return true;
    }
    return false;
}

void getSimpleTime(simpleTime *currentTime){
    currentTime->hour = timeClient.getHours();
    currentTime->minute = timeClient.getMinutes();
}

uint32_t getEpochTime(){
    return timeClient.getEpochTime();
}

bool updateTimeClient(){
    timeClient.update();
    if(timeClient.isTimeSet()){
        return true;
    }
    return false;
}


bool setTimerMilliseconds(uint8_t milliseconds){
    uint32_t timeNow = millis();
    uint32_t millisecondsPassed = timeNow - timeLast;
    if(millisecondsPassed >= milliseconds){
        timeLast = timeNow;
        return true;
    }
    return false;
}

void setTimerMillisecondsCallback(uint8_t milliseconds, void (*callbackFunction)()){
    if(setTimerMilliseconds(milliseconds)){
        (*callbackFunction)();
    }
}


bool setTimerSeconds(uint8_t seconds){
    uint32_t timeNow = millis()/1000;
    uint8_t secondsPassed = timeNow - timeLast;
    // assuming the method gets called in loop and a loop does not take longer than 255 seconds
    if(secondsPassed >= seconds){
        timeLast = timeNow;
        updateTimeClient();
        return true;
    }
    return false;
}


void setTimerSecondsCallback(uint8_t seconds, void (*callbackFunction)()){
    if(setTimerSeconds(seconds)){
        (*callbackFunction)();
    }

}

// millis loops at around 72min, so dont go over that
bool setTimerMinutes(uint8_t minutes){
    uint32_t timeNow = millis()/1000;
    uint32_t secondsPassed = timeNow - timeLast;
    uint8_t minutesPassed = secondsPassed / 60;
    if(minutesPassed >= minutes){
        timeLast = timeNow;
        updateTimeClient();
        return true;
    }
    return false;
}

void setTimerMinutesCallback(uint8_t minutes, void (*callbackFunction)()){
    if(setTimerMinutes(minutes)){
        (*callbackFunction)();
    }
}
