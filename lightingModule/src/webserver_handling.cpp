#include "webserver_handling.h"
#include "internet_settings.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

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

void lightingOn(AsyncWebServerRequest* request){
    lightingStatus = true;

    Serial.printf("lightingStatus = %d\n", lightingStatus);

    request->send(200);
}

void lightingOff(AsyncWebServerRequest* request){
    lightingStatus = false;

    Serial.printf("lightingStatus = %d\n", lightingStatus);

    request->send(200);
}

void raiseBrightness(AsyncWebServerRequest* request){
    if(lightingBrightness < 3) lightingBrightness++;

    Serial.printf("brightness = %d\n", lightingBrightness);

    request->send(200);

}

void lowerBrightness(AsyncWebServerRequest* request){
    if(lightingBrightness > 0) lightingBrightness--;

    Serial.printf("brightness = %d\n", lightingBrightness);

    request->send(200);

}

void handleUnkownRequest(AsyncWebServerRequest* request){
    request->send(404);
}

void initWebserver(){
    if(WiFi.status() != WL_CONNECTED){
        return;
    }

    server.on("/lightingOn", lightingOn);
    server.on("/lightingOff", lightingOff);
    server.on("/raiseBrightness", raiseBrightness);
    server.on("/lowerBrightness", lowerBrightness);
    server.begin();
}