#include <ESP8266WiFi.h>
#include "wl_definitions.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "time_handling.h"

unsigned long timeNow = 0;
unsigned long timeLast = 0;
uint8_t secondsPassed = 0;
uint8_t millisecondsPassed = 0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 120000);

bool buildTimeConnection(){
    if(WiFi.status() == WL_CONNECTED){
        timeClient.begin();
        timeClient.setTimeOffset(7200);
        timeClient.update();
        return true;
    }
    return false;
}

void getSimpleTime(struct simpleTime *currentTime){
    currentTime->hour = timeClient.getHours();
    currentTime->minute = timeClient.getMinutes();
}

void updateTimeClient(){
    timeClient.update();
}

bool setTimerMilliseconds(uint8_t milliseconds){
    timeNow = millis();
    millisecondsPassed = timeNow - timeLast;
    if(millisecondsPassed >= milliseconds){
        timeLast = timeNow;
        return true;
    }
    return false;

}

bool setTimerSeconds(uint8_t seconds){
    timeNow = millis()/1000;
    secondsPassed = timeNow - timeLast;
    if(secondsPassed >= seconds){
        // this makes no sense
        timeLast = timeNow;
        updateTimeClient();
        return true;
    }
    return false;
}
