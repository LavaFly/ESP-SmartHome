#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>

#include "webserver_handling.h"
#include "sensor_handling.h"
#include "internet_settings.h"

#define POWERPIN 5

AsyncWebServer server(80);

uint8_t buildRouterConnection(){

    WiFi.begin(SSID, PASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        Serial.println("Connected to local network");
        Serial.println(WiFi.localIP());
        return 1;
    }

    WiFi.begin(APSSID, APPASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        Serial.println("Connected to ap network");
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
    server.on("/pcStatus", handleStatusRequest);
    server.on("/on", handlePowerOn);
    server.on("/off", handlePowerOff);
    server.on("/currentReading", handleSensorReading);
    server.on("/json", handleJSONRequest);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "http://base.local");
    server.begin();
    ArduinoOTA.begin();
    return 1;
}

void loopOTA(){
    ArduinoOTA.handle();
}

uint8_t setupMDNS(){
    if(!MDNS.begin("pc")){
        Serial.println("Error setting up mDNS responder!");
        return 0;
    }
    Serial.println("mDNS responder started");

    // assumes the server has been started, but should be checked for
    MDNS.addService("http", "tcp", 80);
    return 1;
}

void handleHTMLRequest(AsyncWebServerRequest *request){
    Serial.printf("got request");
    request->send(200);
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

void handlePowerOn(AsyncWebServerRequest *request){
    Serial.println("powerOn");
    // first check if already on
    request->send(200);
    digitalWrite(POWERPIN, HIGH);
    delay(100); // delays during a web response are very bad!
    digitalWrite(POWERPIN, LOW);
}

void handlePowerOff(AsyncWebServerRequest *request){
    Serial.println("powerOff");
    // first check if already off
    request->send(200);
    digitalWrite(POWERPIN, HIGH);
    delay(100); // delays during a web response are very bad!
    digitalWrite(POWERPIN, LOW);
}

void handleSensorReading(AsyncWebServerRequest *request){
    Serial.println("Sending sensor data");
    char* sensorData = (char*)malloc(sizeof(char) * 100);

    getSensorReading(sensorData, 100);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", sensorData);
    request->send(response);

    free(sensorData);
}
void handleStatusRequest(AsyncWebServerRequest *request){

}
