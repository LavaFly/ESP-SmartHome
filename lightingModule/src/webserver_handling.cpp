#include "webserver_handling.h"
#include "internet_settings.h"
//#include "sd_handling.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

AsyncWebServer server(80);

bool buildRouterConnection(){
    //Serial.println("Connecting to WiFi");

    WiFi.begin(APSSID, APPASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        //Serial.println("Connected to ap network");
        //Serial.println(WiFi.localIP());
        return true;
    }

    WiFi.begin(SSID, PASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        //Serial.println("Connected to local network");
        //Serial.println(WiFi.localIP());
        return true;
    }

    return false;
}

void setupMDNS(){
    if(!MDNS.begin("lightingModule")){
        //Serial.println("Error setting up mDNS responder!");
        while(1){ delay(1000); }
    }
    //Serial.println("mDNS responder started");

    // assumes the server has been started, but should be checked for
    MDNS.addService("http", "tcp", 80);
}

void lightingOn(AsyncWebServerRequest* request){
    //Serial.println("lighting on");

    sendOnSignal();

    request->send(200);
}

void lightingOff(AsyncWebServerRequest* request){
    //Serial.println("lighting off");

    sendOffSignal();

    request->send(200);
}

void raiseBrightness(AsyncWebServerRequest* request){
    //Serial.println("lighting raise");

    sendBrighterSignal();

    request->send(200);
}

void lowerBrightness(AsyncWebServerRequest* request){
    //Serial.println("lighting lower");

    sendDarkerSignal();

    request->send(200);
}

void handleUnkownRequest(AsyncWebServerRequest* request){
    request->send(404);
}

void handleLiveStatus(AsyncWebServerRequest *request){
    Serial.println("got liveStatus Request");
    request->send(200);
}


void baseResponse(AsyncWebServerRequest* request){
    //Serial.println("got request");
    request->send(200);
}

void initWebserver(){
    if(WiFi.status() != WL_CONNECTED){
        return;
    }

    server.on("/", baseResponse);
    server.on("/isLive", handleLiveStatus);
    server.on("/lightingOn", lightingOn);
    server.on("/lightingOff", lightingOff);
    server.on("/raiseBrightness", raiseBrightness);
    server.on("/lowerBrightness", lowerBrightness);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    /*
    server.on("/", HTTP_GET, [](AsyncWebserverRequest *request){
              request->send(SD, "/index.html", "text/html");
    });
    server.serveStatic("/", SD, "/");
    */
    server.begin();
    ArduinoOTA.begin();
}

void loopOTA(){
    ArduinoOTA.handle();
}

