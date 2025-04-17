#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
#include "time_handling.h"
#include "voice_handling.h"
#include "led_handling.h"


// Audio                Index
#define VR_PC           0
#define VR_LIGHT        1
#define VR_TIME         2
#define VR_WEATHER      3
#define VR_TEMPERATURE  4
#define VR_CO2          5
#define VR_HUMIDITY     6
#define VR_BRIGHTNESS   7

#define VR_ON           8
#define VR_OFF          9
#define VR_BRIGHTER     10
#define VR_DARKER       11
#define VR_TENT         12

typedef struct {
    //0-254 -> slot
    //255   -> ignore
    uint8_t recordMap[7];

    // 0 -> remain in state till t > ~3s then 1
    // 1 -> call defaultResponse, clear timer, load default recordMap
    uint8_t nodeType;

    // returns nodeType
    // 0 -> call requestUrl, clean
    // 1 -> clean
    uint8_t (*eventResponse)();
} vrEvent;
vrEvent eventMap[14];


// D7 - TX, D8 - RX
VR myVR(13,15);
uint8_t records[7];
uint8_t buf[64];
char requestUrl[64];
uint8_t eventMapIndex;
timerElement* vrTimer;

uint8_t (*EventResponse[12])();
//void (*EventResponse[12])(void (*responseCallback)());


void initVR(){
    myVR.begin(9600);
    delay(100);
    if(myVR.clear() == 0){
        Serial.println("VR cleared");
    } else {
        Serial.println("VR not cleared");
    }

    loadDefaultResponse();
}


void loadDefaultResponse(){
    myVR.clear();

    records[0] = VR_PC;
    records[1] = VR_LIGHT;
    records[2] = VR_TIME;
    records[3] = VR_WEATHER;
    records[4] = VR_TEMPERATURE;
    records[5] = VR_CO2;

    myVR.load(records, 6);
}

bool loadEventResponse(uint8_t indexOfRecord){
    uint8_t emptyRecordMap = true;
    for(uint8_t i = 0; i < 7; i++){
        // 255 = empty
        if(eventMap[indexOfRecord].recordMap[i] != 255){
            records[i] = eventMap[indexOfRecord].recordMap[i];
            Serial.print("i = ");
            Serial.print(i);
            Serial.print(" val = ");
            Serial.println(records[i]);
            emptyRecordMap = false;
        }
    }
    if(myVR.load(records, 7) == -1){
        Serial.println("load unsuccesssfulllll");
    }
    return emptyRecordMap;
}

void loadOnOff(){
    myVR.clear();

    // i should clear the rest of records but dont
    records[0] = VR_ON;
    records[1] = VR_OFF;

    myVR.load(records, 2);
}

void loadOnOffBrighterDarker(){
    myVR.clear();

    // i should clear the rest of records but dont
    records[0] = VR_ON;
    records[1] = VR_OFF;
    records[2] = VR_BRIGHTER;
    records[3] = VR_DARKER;

    myVR.load(records, 4);
}

void handleVR(){
    int ret = myVR.recognize(buf, 50);
    // set ret by Serial input
    if(Serial.available() > 0){
        ret = Serial.readStringUntil('\n').toInt();
    }
    if(ret > 0){
        // buf[2] contains record index ( > 7 )
        // buf[3] contains length of signature if present
        // buf[4 - x] contains the signature

        uint8_t indexOfRecord = buf[2];
        while(Serial.available() == 0){
        }
        indexOfRecord = Serial.readStringUntil('\n').toInt();
        // set indexOfRecord by serial input
        eventMapIndex = indexOfRecord;
        //  load new records / call EventRespose
        uint8_t isLeaf = eventMap[indexOfRecord].nodeType;

        if(!isLeaf){
            //vrTimerIndex = setTimerSeconds(3);
            vrTimer = addTimer(3);
            loadEventResponse(indexOfRecord);
        }

        // timer must exist and not have run out, otherwise leave function
        //if(vrTimerIndex != 0 && !checkTimer(vrTimerIndex)){
            //return;
        //}
        if(vrTimer != NULL){
            Serial.println("hel");
        }
        if(!checkTimer(vrTimer)){
            Serial.println("hjerhje");
        }
        if(vrTimer != NULL && !checkTimer(vrTimer)){
            Serial.println("this debug");
            return;
        }

        eventMap[eventMapIndex].eventResponse();
        deleteTimer(vrTimer);
        eventMapIndex = 0;

        loadDefaultResponse();
    }

    if(vrTimer != NULL){
        Serial.println("1");
        // check timer
        if(checkTimer(vrTimer)){
            Serial.println("2");
            Serial.println(eventMapIndex);
            eventMap[eventMapIndex].eventResponse();
            Serial.println("3");
            deleteTimer(vrTimer);
            Serial.println("4");
            eventMapIndex = 0;
            vrTimer = NULL;
        }
    }
}


void addToEventReponse(uint8_t index, uint8_t (*response)()){
    EventResponse[index] = response;
}


unsigned long vrClearTimer = 0;


// all of this will be moved to vr_handling at some point
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

void testEventResponse(){
    eventMap[1].eventResponse();
    eventMap[2].eventResponse();
}

void setupEventMap(){
    eventMap[0].recordMap[0] = VR_ON;
    eventMap[0].recordMap[1] = VR_OFF;
    eventMap[0].nodeType = 0;
    eventMap[0].eventResponse = vr_pc;


    eventMap[1].recordMap[0] = VR_ON;
    eventMap[1].recordMap[1] = VR_OFF;
    eventMap[1].recordMap[2] = VR_BRIGHTER;
    eventMap[1].recordMap[3] = VR_DARKER;
    eventMap[1].nodeType = 0;
    eventMap[1].eventResponse = vr_light;


    eventMap[2].nodeType = 1;
    eventMap[2].eventResponse = vr_time;

    eventMap[3].nodeType = 1;
    eventMap[3].eventResponse = vr_weather;

    eventMap[4].recordMap[0] = VR_TENT;
    eventMap[4].nodeType = 0;
    eventMap[4].eventResponse = vr_weather;

    eventMap[5].recordMap[0] = VR_TENT;
    eventMap[5].nodeType = 1;
    eventMap[5].eventResponse = vr_empty;


    eventMap[6].recordMap[0] = VR_TENT;
    eventMap[6].nodeType = 1;
    eventMap[6].eventResponse = vr_co2;


    eventMap[6].recordMap[0] = VR_TENT;
    eventMap[6].nodeType = 1;
    eventMap[6].eventResponse = vr_empty;
}

uint8_t vr_pc(){
    Serial.println("pc");
    strcpy(requestUrl, "http://pc.local/");
    return 0;
}
uint8_t vr_light(){
    Serial.println("light");
    strcat(requestUrl, "raiseBrightness");
    loadOnOffBrighterDarker();
    vrClearTimer = millis();
    return 0;

}
uint8_t vr_time(){
    Serial.println("time");
    strcpy(requestUrl, "http://base.local/showTime");
    // actually just show time without web request, this is just a placeholder
    return 0;
}
uint8_t vr_weather(){
    Serial.println("weather");
    // same here
    return 0;

}
uint8_t vr_temperature(){
    Serial.println("temperature");
    return 0;

}
uint8_t vr_co2(){
    Serial.println("co2");
    return 0;

}
uint8_t vr_on(){
    Serial.println("on");
    strcat(requestUrl, "on");
    return 0;
}
uint8_t vr_off(){
    Serial.println("off");
    strcat(requestUrl, "off");
    return 0;
}
uint8_t vr_brighter(){
    Serial.println("brighter");
    strcat(requestUrl, "raiseBrightness");
    //httpGetRequestIgnoreResponse("http://lightingModule.local/raiseBrightness");
    return 0;
}
uint8_t vr_darker(){
    Serial.println("darker");
    strcat(requestUrl, "lowerBrightness");
    return 0;
}
uint8_t vr_empty(){
    Serial.println("empty");
    return 0;
}

bool checkVrTimer(uint8_t vrResetTimeSeconds){
    if(vrClearTimer != 0 && millis() > vrClearTimer + vrResetTimeSeconds){
        return true;
    }
    return false;
}

void clearVrTimer(){
    vrClearTimer = 0;
}
