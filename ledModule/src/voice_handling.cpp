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
VR myVR(14,12);
uint8_t records[7];
uint8_t buf[64];
char requestUrl[64];
uint8_t eventMapIndex;
timerElement* vrTimer;
uint8_t currentEvent = 0;

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

    if(myVR.load(records, 6) == -1){
        Serial.println("bad load inside vr");
    }
}

bool loadEventResponse(uint8_t indexOfRecord){
    uint8_t emptyRecordMap = true;
    for(uint8_t i = 0; i < 7; i++){
        // 255 = empty
        if(eventMap[indexOfRecord].recordMap[i] != 255){
            records[i] = eventMap[indexOfRecord].recordMap[i];
            emptyRecordMap = false;
        }
    }
    if(myVR.load(records, 7) == -1){
        Serial.println("load unsuccesssfulllll");
    }
    return emptyRecordMap;
}

// delete
void loadOnOff(){
    myVR.clear();

    // i should clear the rest of records but dont
    records[0] = VR_ON;
    records[1] = VR_OFF;

    myVR.load(records, 2);
}

// delete
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
    // i am aware that this method can be simplified and that
    // there is redundancy and will fix this at some point
    // but
    // works for now
    int ret = 0;
    ret = myVR.recognize(buf, 50);


    if(Serial.available() > 0){
        ret = Serial.parseInt();
        Serial.print("pressed = ");
        Serial.println(ret);

    // currently testing via serial
    //if(ret > 0){
        // buf[2] contains record index ( > 7 )
        // buf[3] contains length of signature if present
        // buf[4 - x] contains the signature

        uint8_t indexOfRecord = buf[2];
        // this is confusing and unnecessary
        indexOfRecord = ret;
        // set indexOfRecord by serial input
        eventMapIndex = indexOfRecord;
        //  load new records / call EventRespose
        uint8_t isLeaf = eventMap[indexOfRecord].nodeType;

        if(!isLeaf){
            vrTimer = resetTimer(vrTimer, 3);
            loadEventResponse(indexOfRecord);
            if(currentEvent != indexOfRecord){
                // transition from node to somewhere new, so manipulate url
                eventMap[eventMapIndex].eventResponse();
            }
            return;
        }

        currentEvent = indexOfRecord;
        if(vrTimer != NULL){
            deleteTimer(vrTimer);
            vrTimer = NULL;
        }

        eventMap[eventMapIndex].eventResponse();
        Serial.print("url = ");
        Serial.println(requestUrl);
        requestUrl[0] = '\0';
        eventMapIndex = 0;

        // reset requestUrl
        loadDefaultResponse();
    }

    if(vrTimer != NULL){
        // check timer
        if(checkTimer(vrTimer)){
            eventMap[eventMapIndex].eventResponse();
            Serial.print("url = ");
            Serial.println(requestUrl);
            requestUrl[0] = '\0';
            deleteTimer(vrTimer);
            eventMapIndex = 0;
            vrTimer = NULL;
        }
    }
}


// delete
void addToEventReponse(uint8_t index, uint8_t (*response)()){
    EventResponse[index] = response;
}


unsigned long vrClearTimer = 0;


// delete
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

    eventMap[7].recordMap[0] = VR_TENT;
    eventMap[7].nodeType = 1;
    eventMap[7].eventResponse = vr_empty;

    eventMap[8].nodeType = 1;
    eventMap[8].eventResponse = vr_on;

    eventMap[9].nodeType = 1;
    eventMap[9].eventResponse = vr_off;

    eventMap[10].nodeType = 1;
    eventMap[10].eventResponse = vr_brighter;

    eventMap[11].nodeType = 1;
    eventMap[11].eventResponse = vr_darker;

    eventMap[12].nodeType = 1;
    eventMap[12].eventResponse = vr_empty;
}

uint8_t vr_pc(){
    Serial.println("pc");
    strcpy(requestUrl, "http://pc.local/");
    return 0;
}
uint8_t vr_light(){
    Serial.println("light");
    strcpy(requestUrl, "http://led.local/");
    Serial.print("set to = ");
    Serial.println(requestUrl);
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
    // show time
    return 0;

}
uint8_t vr_co2(){
    Serial.println("co2");
    // show co2
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
    Serial.print("str = ");
    Serial.println(requestUrl);
    strcat(requestUrl, "raiseBrightness");
    Serial.print("afterstr = ");
    Serial.println(requestUrl);
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
