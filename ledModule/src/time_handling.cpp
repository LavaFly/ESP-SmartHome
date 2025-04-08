#include <ESP8266WiFi.h>
#include "wl_definitions.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "time_handling.h"


// this should be a linked list
unsigned long timeLast = 0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "base.local", 3600, 120000);//europe.pool.ntp.org

typedef struct {
    uint32_t timeStamp;
    uint8_t durationSeconds;
} timerElement;

bool buildTimeConnection(){
    if(WiFi.status() == WL_CONNECTED){
        timeClient.begin();
        timeClient.setTimeOffset(3600);//7200
        timeClient.update();
        return true;
    }
    return false;
}

void getSimpleTime(struct simpleTime *currentTime){
    currentTime->hour = timeClient.getHours();
    currentTime->minute = timeClient.getMinutes();
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



// these will be changed soon, only placeholder for proper implementation
bool checkTimer(uint8_t timerIndex){
    return false;
}

uint8_t addTimer(uint8_t seconds){
    return 0;
}
uint8_t deleteTimer(uint8_t timerIndex){
    return 0;
}
uint8_t resetTimer(uint8_t timerIndex){
    return 0;
}
bool setTimerSeconds(uint8_t seconds){
    uint32_t timeNow = millis()/1000;
    uint8_t secondsPassed = timeNow - timeLast;
    // assuming the method gets called in a loop and the loop does not take longer than 255 seconds
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
