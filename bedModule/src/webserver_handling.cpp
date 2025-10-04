#include "webserver_handling.h"
#include "internet_settings.h"
//#include "sd_handling.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

AsyncWebServer server(80);
WiFiClient client;
HTTPClient http;
WebSocketsClient webSocket;

uint8_t isConnected;

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
    if(!MDNS.begin("bed")){
        return 0;
    }
    //Serial.println("mDNS responder started");

    // assumes the server has been started, but should be checked for
    MDNS.addService("http", "tcp", 80);
    return 1;
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

uint8_t initWebserver(){
    if(WiFi.status() != WL_CONNECTED){
        return 0;
    }

    server.on("/", baseResponse);
    server.on("/isLive", handleLiveStatus);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "http://base.local");
    server.begin();
    ArduinoOTA.begin();
    return 1;
}

void loopOTA(){
    ArduinoOTA.handle();
}

uint8_t httpGetRequestIgnoreResponse(const char* path){
    uint8_t retCode = 0;
    if(http.begin(client, path)){
        int httpCode = http.GET();
        if(httpCode == HTTP_CODE_OK){
            retCode = 1;
        }
        http.end();
    }
    return retCode;
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length){
    switch(type){
        case WStype_DISCONNECTED:
            isConnected = false;
            break;
        case WStype_CONNECTED:
            isConnected = true;
            break;
    }
}

void webSocketSend(char* inputText){
    // connect to webSocket
    webSocket.begin("ws://led.local", 81, "/");
    delay(50);
    if(isConnected){
        webSocket.sendTXT(inputText);
    } else {
        Serial.println("not connected");
    }
}

const String* httpGetRequest(const char* path){
    if(http.begin(client, path)){
        int httpCode = http.GET();
        if(httpCode == HTTP_CODE_OK){
           return &http.getString();
        }
        http.end();
    }
    return 0;
}


WiFiClient& httpGetRequestStream(const char* path){
    http.useHTTP10(true);
    http.begin(client, path);
    http.GET();
    return http.getStream();
}

void httpEndRequestStream(){
    http.end();
}

