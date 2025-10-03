#include "webserver_handling.h"
#include "internet_settings.h"
//#include "sd_handling.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

AsyncWebServer server(80);

uint8_t buildRouterConnection(){

    WiFi.begin(SSID, PASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        //Serial.println("Connected to local network");
        //Serial.println(WiFi.localIP());
        return 1;
    }

    WiFi.begin(APSSID, APPASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        //Serial.println("Connected to ap network");
        //Serial.println(WiFi.localIP());
        return 1;
    }

    return 0;
}

uint8_t setupMDNS(){
    if(!MDNS.begin("lighting")){
        return 0;
    }
    //Serial.println("mDNS responder started");

    // assumes the server has been started, but should be checked for
    MDNS.addService("http", "tcp", 80);
    return 1;
}

void lightingOn(AsyncWebServerRequest* request){
    sendOnSignal();

    request->send(200);
}

void lightingOff(AsyncWebServerRequest* request){
    sendOffSignal();

    request->send(200);
}

void raiseBrightness(AsyncWebServerRequest* request){
    sendBrighterSignal();

    request->send(200);
}

void lowerBrightness(AsyncWebServerRequest* request){
    sendDarkerSignal();

    request->send(200);
}

void handleUnkownRequest(AsyncWebServerRequest* request){
    request->send(404);
}

void handleLiveStatus(AsyncWebServerRequest *request){
    request->send(200);
}


void baseResponse(AsyncWebServerRequest* request){
    request->send(200);
}

void handleOne(AsyncWebServerRequest* request){
    setBrightness(1);
    request->send(200);
}
void handleTwo(AsyncWebServerRequest* request){
    setBrightness(2);
    request->send(200);
}
void handleThree(AsyncWebServerRequest* request){
    setBrightness(3);
    request->send(200);
}
void handleFour(AsyncWebServerRequest* request){
    setBrightness(4);
    request->send(200);
}
void handleToggle(AsyncWebServerRequest* request){
    toggleLight();
    request->send(200);
}

uint8_t initWebserver(){
    if(WiFi.status() != WL_CONNECTED){
        return 0;
    }

    server.on("/", baseResponse);
    server.on("/isLive", handleLiveStatus);
    server.on("/on", lightingOn);
    server.on("/off", lightingOff);
    server.on("/raiseBrightness", raiseBrightness);
    server.on("/lowerBrightness", lowerBrightness);
    server.on("/toggle", handleToggle);

    // this is quite ugly but good enough, i just want something working
    // will properly parse the request url at some later point
    // but my lighting will never have more than 4 brightness levels
    server.on("/1", handleOne);
    server.on("/2", handleTwo);
    server.on("/3", handleThree);
    server.on("/4", handleFour);

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "http://base.local");
    server.begin();
    ArduinoOTA.begin();
    return 1;
}

void loopOTA(){
    ArduinoOTA.handle();
}

