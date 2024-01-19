#include "webserver_handling.h"
#include "webpage.h"
#include "internet_settings.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <NTPClient.h>


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 120000);
AsyncWebServer server(80);


void buildRouterConnection(){
    Serial.println("Connecting to WiFi");

    WiFi.begin(SSID, PASS);
    while(WiFi.status() != WL_CONNECTED){
        Serial.print("-");
        delay(100);
    }
    Serial.println(WiFi.localIP());
}

void initWebserver(){
    if(WiFi.status() != WL_CONNECTED){
        return;
    }
    server.on("/", handleHTMLRequest);
    server.on("/pcStatus", handleStatusRequest);
    server.on("/pcPowerOn", handlePowerOn);
    server.begin();
}

void buildTimeConnection(){
    timeClient.begin();
    timeClient.setTimeOffset(3600);
    timeClient.update();
}

void setupMDNS(){
    if(!MDNS.begin("pcModule")){
        Serial.println("Error setting up mDNS responder!");
        while(1){ delay(1000); }
    }
    Serial.println("mDNS responder started");

    // assumes the server has been started, but should be checked for
    MDNS.addService("http", "tcp", 80);
}

void getSimpleTime(struct simpleTime *currentTime){
    currentTime->hour = timeClient.getHours();
    currentTime->minute = timeClient.getMinutes();
}

void handleHTMLRequest(AsyncWebServerRequest *request){
    Serial.printf("got request");
    request->send(200);
}

void handleJSONRequest(AsyncWebServerRequest *request){

}

void handlePowerOn(AsyncWebServerRequest *request){

}

void handleStatusRequest(AsyncWebServerRequest *request){

}
