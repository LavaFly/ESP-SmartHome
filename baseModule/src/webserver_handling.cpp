#include "webserver_handling.h"
#include "webpage.h"
#include "internet_settings.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 120000);

void buildRouterConnection(){
    Serial.println("Connecting to WiFi");

    WiFi.begin(SSID, PASS);
    while(WiFi.status() != WL_CONNECTED){
        Serial.print("-");
        delay(100);
    }
    Serial.println(WiFi.localIP());
}

void buildTimeConnection(){
    timeClient.begin();
    timeClient.setTimeOffset(3600);
    timeClient.update();
}

void getSimpleTime(struct simpleTime *currentTime){
    currentTime->hour = timeClient.getHours();
    currentTime->minute = timeClient.getMinutes();
}

void handleHTMLRequest(AsyncWebServerRequest *request){

}

void handleJSONRequest(AsyncWebServerRequest *request){

}
