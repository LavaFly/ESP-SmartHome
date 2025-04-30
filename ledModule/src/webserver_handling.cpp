#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include "webserver_handling.h"
#include "internet_settings.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
WiFiClient client;
HTTPClient http;

// only for ap-mode
IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

extern void setupTextAnimation(String message);
extern void showTime();

uint8_t buildRouterConnection(){
    WiFi.begin(APSSID, APPASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        Serial.println("Connected to ap network");
        Serial.println(WiFi.localIP());
        return 1;
    }

    WiFi.begin(SSID, PASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        Serial.println("Connected to local network");
        Serial.println(WiFi.localIP());
        return 1;
    }

    return 0;
}

uint8_t initWebserver(){
    if(WiFi.status() != WL_CONNECTED){
        return 0;
    }

    ws.onEvent(onEvent);
    server.addHandler(&ws);
    server.on("/", handleHTMLRequest);
    server.on("/isLive", handleLiveStatus);
    server.on("/showTime", handleTimeRequest);
    server.on("/showTemperature", handleTemperatureRequest);
    server.on("/showCO2", handleCO2Request);
    server.on("/json", handleJSONRequest);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "http://base.local");
    server.begin();
    ArduinoOTA.begin();
    return 1;
}

void loopOTA(){
    ArduinoOTA.handle();
}


uint8_t setupMDNS(){
    if(!MDNS.begin("led")){
        return 0;
    }

    // assumes the server has been started, but should be checked for
    MDNS.addService("http", "tcp", 80);
    return 1;
}

void handleHTMLRequest(AsyncWebServerRequest *request){
    request->send(200);
}

void handleLiveStatus(AsyncWebServerRequest *request){
    request->send(200);
}

void handleTimeRequest(AsyncWebServerRequest *request){
    showTime();
    request->send(200);
}

void handleJSONRequest(AsyncWebServerRequest *request){
    // apart from a photodiode, this module has no sensors
    // but may get some in the future, idk
}

void handleTemperatureRequest(AsyncWebServerRequest *request){
    // display temperature
}
void handleCO2Request(AsyncWebServerRequest *request){
    // display co2
}

uint8_t httpGetRequestIgnoreResponse(const char* path){
    if(http.begin(client, path)){
        int httpCode = http.GET();
        if( httpCode == HTTP_CODE_OK){
            return 1;
        }
        http.end();
    }
    return 0;
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

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        //Serial.print("got message:  ");
        //Serial.println((char*)data);
        setupTextAnimation((char*)data);
    }
}
void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

uint8_t cleanUpSockets(){
    // check if this can fail
    ws.cleanupClients();
    return 1;
}
