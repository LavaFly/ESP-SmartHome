#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>

#include "webserver_handling.h"
#include "sensor_handling.h"
#include "internet_settings.h"

#define POWERPIN 5

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
    server.on("/pcStatus", handleStatusRequest);
    server.on("/pcPowerOn", handlePowerOn);
    server.on("/currentReading", handleSensorReading);
    server.on("/json", handleJSONRequest);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "http://pc.local");
    server.begin();
    ArduinoOTA.begin();
}

void loopOTA(){
    ArduinoOTA.handle();
}

void setupMDNS(){
    if(!MDNS.begin("pc")){
        Serial.println("Error setting up mDNS responder!");
        while(1){ delay(1000); }
    }
    Serial.println("mDNS responder started");

    // assumes the server has been started, but should be checked for
    MDNS.addService("http", "tcp", 80);
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
    request->send(200);
    digitalWrite(POWERPIN, HIGH);
    delay(100);
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
