#include <ESP8266WiFi.h>
#include "wl_definitions.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "time_handling.h"


// this should be a linked list
unsigned long timeLast = 0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "base.local", 0, 120000);//europe.pool.ntp.org

typedef struct timerElement{
    uint32_t timeStamp;
    struct timerElement* next;
    uint8_t durationSeconds;
    uint8_t hasRunOut;
} timerElement;

timerElement* head = nullptr;

bool buildTimeConnection(){
    if(WiFi.status() == WL_CONNECTED){
        timeClient.begin();
        timeClient.setTimeOffset(0);//7200
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


timerElement* addTimer(uint8_t seconds){
    timerElement *newTimer = (timerElement *)malloc(sizeof(timerElement));
    if(newTimer == NULL){
        Serial.println("a great big fuck up");
    }
    newTimer->timeStamp = millis();
    newTimer->durationSeconds = seconds;

    if(head == NULL){
        head = newTimer;
    } else {
        //append to end of list
        timerElement *iterator = head->next;
        while(iterator != NULL){
            iterator = iterator->next;
        }
        iterator->next = newTimer;
    }
    return newTimer;
}

bool checkTimer(timerElement* timerPtr){
    uint32_t currentTime = millis();

    if(timerPtr->hasRunOut || timerPtr->timeStamp + timerPtr->durationSeconds * 1000 < currentTime){
        timerPtr->hasRunOut = true;
        return true;
    }
    return false;
}

uint8_t deleteTimer(timerElement* timerPtr){
    // traverse list until ptr is found
    timerElement *iterator = head;
    if(head == NULL){
        Serial.println("bad");
        return 0;
    }

    if(head->next == NULL){
        Serial.println("also bad");
        return 0;
    }

    while(iterator->next != timerPtr){
        iterator = iterator->next;
    }
    timerElement *prev = iterator;
    timerElement *post = iterator->next->next;

    prev->next = post;

    free(timerPtr);

    return 1;
}

timerElement* resetTimer(timerElement* timerPtr){
    timerPtr->hasRunOut = false;
    timerPtr->timeStamp = millis();
    return timerPtr;
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
