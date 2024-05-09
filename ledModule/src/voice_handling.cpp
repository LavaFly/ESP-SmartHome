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
