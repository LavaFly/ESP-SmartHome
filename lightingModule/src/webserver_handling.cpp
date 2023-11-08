#include "webserver_handling.h"
#include "internet_settings.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

void buildRouterConnection(){
    Serial.println("Connecting to WiFi");

    WiFi.begin(SSID, PASS);
    while(WiFi.status() != WL_CONNECTED){
        Serial.print("-");
        delay(100);
    }
    Serial.println(WiFi.localIP());
}

void handleHTMLRequest(AsyncWebServerRequest *request){

}

void handleJSONRequest(AsyncWebServerRequest *request){

}
