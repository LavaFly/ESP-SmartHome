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
    } else {
        Serial.println("Starting AP-Mode");
        WiFi.softAPConfig(local_IP, gateway, subnet);
        WiFi.softAP(APSSID, APPASS, 1, 7);
        Serial.println(WiFi.softAPIP());
        return true;
    }
    return false;
}

void initWebserver(){
    if(WiFi.status() != WL_CONNECTED){
        return;
    }
    server.on("/", handleHTMLRequest);
    server.begin();
}

void buildTimeConnection(){
    timeClient.begin();
    timeClient.setTimeOffset(3600);
    timeClient.update();
}

void setupMDNS(){
    if(!MDNS.begin("baseModule")){
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
