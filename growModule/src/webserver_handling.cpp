#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>

#include "webserver_handling.h"
#include "sensor_handling.h"
#include "actuator_handling.h"
#include "internet_settings.h"


AsyncWebServer server(80);

// check if needed?
extern uint8_t getSensorReading(char *formattedResponse, size_t maxResponseLen);
extern uint8_t activateActuator();
extern void deactivateActuator();
extern uint8_t pumpActive;
extern uint8_t pumpWorkaround;

uint8_t buildRouterConnection(){
    Serial.println("Connecting to WiFi");

    WiFi.begin(APSSID, APPASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        Serial.println("Connected to ap network");
        Serial.println(WiFi.localIP());
        return 1;
    }

    WiFi.begin(SSID, PASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        Serial.println("Connected to local network");
        Serial.println(WiFi.localIP());
        return 1;
    }

    return 0;
}

uint8_t initWebserver(){
    if(WiFi.status() != WL_CONNECTED){
        return 0;
    }
    server.on("/isLive", handleLiveStatus);
    // calling this url will crash the mc, i will fix this someday
    server.on("/currentReading", handleSensorReading);
    server.on("/json", handleJSONRequest);

    // this can be done better, i know, but will work for now
    server.on("/pump/on", handlePumpOn);
    server.on("/pump/off", handlePumpOff);
    server.on("/pump/ml", handlePumpMilliLiter);

    server.on("/light/on", handleLightOn);
    server.on("/light/off", handleLightOff);
    server.on("/light/seconds", handleLightSecondsTimer);


    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "http://base.local");
    server.begin();
    ArduinoOTA.begin();
    return 1;
}

void loopOTA(){
    ArduinoOTA.handle();
}


uint8_t setupMDNS(){
    if(!MDNS.begin("grow")){
        Serial.println("Error setting up mDNS responder!");
        return 0;
    }
    Serial.println("mDNS responder started");

    // assumes the server has been started, but should be checked for
    MDNS.addService("http", "tcp", 80);
    return 1;
}

void handleLiveStatus(AsyncWebServerRequest *request){
    request->send(200);
}

void handleJSONRequest(AsyncWebServerRequest *request){
    int numberOfCurrentReadings = getNumOfReadingsInList();
    char* sensorData = (char*)malloc(sizeof(char) * 180); // rougly 124 will be used
                                                          // check if null

    AsyncResponseStream *response = request->beginResponseStream("application/json");

    response->print("[");
    for(uint8_t i = 0; i < numberOfCurrentReadings; i++){
        getSensorReadingFromList(sensorData, 180, i);
        response->print(sensorData);
        if(i != numberOfCurrentReadings - 1){
            response->print(",");
        }
    }
    response->print("]");
    request->send(response);
    free(sensorData);
}
void handleSensorReading(AsyncWebServerRequest *request){
    char* sensorData = (char*)malloc(sizeof(char) * 180); // rougly 124 will be used

    getSensorReading(sensorData, 180);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", sensorData);

    request->send(response);

    free(sensorData);
}



void handlePumpOn(AsyncWebServerRequest *request){
    activateActuator();
    request->send(200);
}

void handlePumpOff(AsyncWebServerRequest *request){
    deactivateActuator();
    request->send(200);
}

void handlePumpMilliLiter(AsyncWebServerRequest *request){
    pumpActive = 1;
    pumpWorkaround = 20;
}


// these will be implemented soon, wanted to get the rough outline first
void handleLightOn(AsyncWebServerRequest *request){

}

void handleLightOff(AsyncWebServerRequest *request){

}

void handleLightSecondsTimer(AsyncWebServerRequest *request){

}
