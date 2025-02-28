#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
#include "voice_handling.h"


// Audio                Index
#define VR_PC           0
#define VR_LIGHT        1
#define VR_TIME         2
#define VR_WEATHER      3
#define VR_TEMPERATURE  4
#define VR_CO2          5

#define VR_ON           8
#define VR_OFF          9
#define VR_BRIGHTER     10
#define VR_DARKER       11

// D1 - TX, D2 - RX
VR myVR(5,4);
uint8_t records[7];
uint8_t buf[64];

void (*EventResponse[12])();
//void (*EventResponse[12])(void (*responseCallback)());


void initVR(){
    myVR.begin(9600);
    delay(100);
    if(myVR.clear() == 0){
        Serial.println("VR cleared");
    } else {
        Serial.println("VR not cleared");
    }

    loadDefaultVR();
}

void loadDefaultVR(){
    myVR.clear();

    records[0] = VR_PC;
    records[1] = VR_LIGHT;
    records[2] = VR_TIME;
    records[3] = VR_WEATHER;
    records[4] = VR_TEMPERATURE;
    records[5] = VR_CO2;

    myVR.load(records, 6);
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
    if(ret > 0){
        // buf[2] contains record index ( < 7 )
        // buf[3] contains length of signature if present
        // buf[4 - x] contains the signature
        int numberOfRecord = buf[1];
        (*EventResponse[numberOfRecord])();
    }
}


void addToEventReponse(uint8_t index, void (*response)()){
    EventResponse[index] = response;
}


unsigned long vrClearTimer = 0;
// 1 = PC, 2 = Light
uint8_t vrTreeBranch = 0;
void setupEventResponse();

void vr_pc();
void vr_light();
void vr_time();
void vr_weather();
void vr_temperature();
void vr_co2();
void vr_on();
void vr_off();
void vr_brighter();
void vr_darker();
void vr_empty();


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

void vr_pc(){
    vrTreeBranch = 1;
    loadOnOff();
    vrClearTimer = millis();
}
void vr_light(){
    vrTreeBranch = 2;
    loadOnOffBrighterDarker();
    vrClearTimer = millis();
}
void vr_time(){
    //showTime();
}
void vr_weather(){
    Serial.println("");

}
void vr_temperature(){
    Serial.println("");

}
void vr_co2(){
    Serial.println("");

}
void vr_on(){
    Serial.println("");
    if(vrTreeBranch == 1){
        //httpGetRequestIgnoreResponse("http://lightingModule.local/pcPowerOn");
    } else if(vrTreeBranch == 2){
        //httpGetRequestIgnoreResponse("http://lightingModule.local/lightingOn");
    } else {

    }
    loadDefaultVR();
}
void vr_off(){
    Serial.println("");
    if(vrTreeBranch == 1){
        Serial.println("not properly implemented, as i havent spliced the necessary cable yet");
        //httpGetRequestIgnoreResponse("http://lightingModule.local/pcPowerOn");
    } else if(vrTreeBranch == 2){
        //httpGetRequestIgnoreResponse("http://lightingModule.local/lightingOff");
    } else {

    }
    loadDefaultVR();
}
void vr_brighter(){
    if(vrTreeBranch == 1){
        Serial.println("impossible");
    } else if(vrTreeBranch == 2){
        //httpGetRequestIgnoreResponse("http://lightingModule.local/raiseBrightness");
    } else {

    }
    loadDefaultVR();
}
void vr_darker(){
    if(vrTreeBranch == 1){
        Serial.println("impossible");
    } else if(vrTreeBranch == 2){
        //httpGetRequestIgnoreResponse("http://lightingModule.local/lowerBrightness");
    } else {

    }
    loadDefaultVR();
}
void vr_empty(){
    loadDefaultVR();
}
