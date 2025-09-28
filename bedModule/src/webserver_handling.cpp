#include "webserver_handling.h"
#include "internet_settings.h"
//#include "sd_handling.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

AsyncWebServer server(80);

uint8_t buildRouterConnection(){
    WiFi.begin(APSSID, APPASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        //Serial.println("Connected to ap network");
        //Serial.println(WiFi.localIP());
        return 1;
    }

    WiFi.begin(SSID, PASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        //Serial.println("Connected to local network");
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

