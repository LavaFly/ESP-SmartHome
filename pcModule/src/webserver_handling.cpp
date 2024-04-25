#include "webserver_handling.h"
#include "internet_settings.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <ElegantOTA.h>

#include <StackThunk.h>


#define POWERPIN 5

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 120000);
AsyncWebServer server(80);

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

void initWebserver(){
    if(WiFi.status() != WL_CONNECTED){
        return;
    }
    ElegantOTA.begin(&server);
    server.on("/", handleHTMLRequest);
    server.on("/isLive", handleLiveStatus);
    server.on("/pcStatus", handleStatusRequest);
    server.on("/pcPowerOn", handlePowerOn);
    server.on("/sensorReading", handleSensorReading);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    server.begin();
}

void loopOTA(){
    ElegantOTA.loop();
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

void handleLiveStatus(AsyncWebServerRequest *request){
    Serial.println("got liveStatus Request");
    request->send(200);
}

void handleJSONRequest(AsyncWebServerRequest *request){

}

void handlePowerOn(AsyncWebServerRequest *request){
    Serial.println("powerOn");
    request->send(200);
    digitalWrite(POWERPIN, HIGH);
    delay(100);
    digitalWrite(POWERPIN, LOW);
}

void handleSensorReading(AsyncWebServerRequest *request){
    Serial.println("Sending sensor data");
    char* sensorData = (char*)malloc(sizeof(char) * 100);

    getSensorReading(sensorData, 100);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", sensorData);
    request->send(response);

    free(sensorData);
}
void handleStatusRequest(AsyncWebServerRequest *request){

}

void updateTimeClient(){
    timeClient.update();
}
