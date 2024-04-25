#include "webserver_handling.h"
#include "internet_settings.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ElegantOTA.h>

AsyncWebServer server(80);
WiFiClient client;
HTTPClient http;

// only for ap-mode
IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

bool buildRouterConnection(){
    Serial.println("Connecting to WiFi");

    WiFi.begin(SSID, PASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        Serial.println("Connected to local network");
        Serial.println(WiFi.localIP());
        return true;
    }
    WiFi.begin(APSSID, APPASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        Serial.println("Connected to ap network");
        Serial.println(WiFi.localIP());
        return true;
    }
    return false;
}

bool initWebserver(){
    if(WiFi.status() != WL_CONNECTED){
        return false;
    }

    ElegantOTA.begin(&server);
    server.on("/", handleHTMLRequest);
    server.on("/isLive", handleLiveStatus);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    server.begin();
    return true;
}

void loopOTA(){
    ElegantOTA.loop();
}


void setupMDNS(){
    if(!MDNS.begin("ledModule")){
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

}

int httpGetRequestIgnoreResponse(const char* path){
    if(http.begin(client, path)){
        int httpCode = http.GET();
        if( httpCode == HTTP_CODE_OK){
            return 1;
        }
        http.end();
    }
    return 0;
}
