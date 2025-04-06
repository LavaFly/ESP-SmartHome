#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>

#include "webserver_handling.h"
#include "sensor_handling.h"
#include "actuator_handling.h"
#include "internet_settings.h"


AsyncWebServer server(80);


bool buildRouterConnection(){
    Serial.println("Connecting to WiFi");

    WiFi.begin(APSSID, APPASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        Serial.println("Connected to ap network");
        Serial.println(WiFi.localIP());
        return true;
    }

    WiFi.begin(SSID, PASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        Serial.println("Connected to local network");
        Serial.println(WiFi.localIP());
        return true;
    }

    return false;
}

void initWebserver(){
    if(WiFi.status() != WL_CONNECTED){
        return;
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


    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "http://grow.local");
    server.begin();
    ArduinoOTA.begin();
}

void loopOTA(){
    ArduinoOTA.handle();
}


void setupMDNS(){
    if(!MDNS.begin("grow")){
        Serial.println("Error setting up mDNS responder!");
        while(1){ delay(1000); }
    }
    Serial.println("mDNS responder started");

    // assumes the server has been started, but should be checked for
    MDNS.addService("http", "tcp", 80);
}

void handleLiveStatus(AsyncWebServerRequest *request){
    Serial.println("got liveStatus Request");
    request->send(200);
}

void handleJSONRequest(AsyncWebServerRequest *request){
    Serial.println("got json request");
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
