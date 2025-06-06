#include <ESP8266WiFi.h>
#include "wl_definitions.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <cstddef>
#include "time_handling.h"


// this should be a linked list
unsigned long timeLast = 0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "base.local", 0, 120000);//europe.pool.ntp.org

typedef struct timerElement{
    uint32_t timeStamp;
    struct timerElement* next;
    uint16_t duration;
    uint8_t inMilliseconds = false;
    uint8_t hasRunOut = false;
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


timerElement* addTimer(uint16_t seconds){
    timerElement *newTimer = (timerElement *)malloc(sizeof(timerElement));
    if(newTimer == NULL){
        Serial.println("a great big fuck up");
    }
    newTimer->timeStamp = millis();
    newTimer->duration = seconds;
    newTimer->hasRunOut = false;
    newTimer->next = NULL;

    if(head == NULL){
        head = newTimer;
        Serial.println("new head");
    } else {
        //append to end of list
        timerElement *iterator = head->next;
        while(iterator != NULL){
            iterator = iterator->next;
        }
        iterator = newTimer;
        Serial.println("new tail");
    }
    return newTimer;
}

timerElement* addTimerMilliseconds(uint16_t milliseconds){
    timerElement *newTimer = (timerElement *)malloc(sizeof(timerElement));
    if(newTimer == NULL){
        Serial.println("very very bad");
    }
    newTimer->timeStamp = millis();
    newTimer->duration = milliseconds;
    newTimer->inMilliseconds = true;
    newTimer->hasRunOut = false;
    newTimer->next = NULL;

    if(head == NULL){
        head = newTimer;
        Serial.println("new head");
    } else {
        // append to end of list
        timerElement *iterator = head->next;
        while(iterator != NULL){
            iterator = iterator->next;
        }
        iterator = newTimer;
        Serial.println("new tail");
    }
    return newTimer;
}

// returns true if timer has run out
bool checkTimer(timerElement* timerPtr){
    uint32_t currentTime = millis();

    if(timerPtr == NULL){
        //Serial.println("timerPtr is null");
        return false;
    }

    if(timerPtr->hasRunOut){
        //Serial.println("flag set");
        return true;
    }

    if(timerPtr->timeStamp + timerPtr->duration * (timerPtr->inMilliseconds ? 1 : 1000) < currentTime){
        //Serial.println("has run out");
        timerPtr->hasRunOut = true;
        return true;
    }

    return false;
}

uint8_t deleteTimer(timerElement* timerPtr){
    timerElement *iterator = head;
    if(head == NULL){
        Serial.println("list is empty/head = null, dangling timer???");
        free(timerPtr);
        return 0;
    }

    if(timerPtr == NULL){
        Serial.println("timerPtr is NULL");
        return 1;
    }

    if(timerPtr == head){
        Serial.println("timer is head");
        if(timerPtr->next != NULL){
            Serial.println("with tail");
            head = timerPtr->next;
            free(timerPtr);
        } else {
            Serial.println("no tail");
            head = NULL;
            free(timerPtr);
        }
        return 1;
    }

    Serial.println("iterating");
    while(iterator->next != timerPtr){
        iterator = iterator->next;
    }

    // iterator->next->next == timerPtr->next
    if(iterator->next->next != NULL){
        Serial.println("next next != NULL");
        iterator->next = timerPtr->next;
    } else {
        Serial.println("not next next != NULL");
        iterator->next = NULL;
    }

    free(timerPtr);

    return 1;
}

timerElement* resetTimer(timerElement* timerPtr){
    if(timerPtr != NULL){
        timerPtr->hasRunOut = false;
        timerPtr->timeStamp = millis();
        return timerPtr;
    }
    return addTimer(3);
}
timerElement* resetTimer(timerElement* timerPtr, uint8_t seconds){
    if(timerPtr != NULL){
        timerPtr->hasRunOut = false;
        timerPtr->timeStamp = millis();
        return timerPtr;
    }
    return addTimer(seconds);
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
