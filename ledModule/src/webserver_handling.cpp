#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <cstdlib>
#include "webserver_handling.h"
#include "internet_settings.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
WiFiClient client;
HTTPClient http;

extern JsonDocument doc;

// only for ap-mode
IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

extern void setupTextAnimation(String message);
extern void showTime();
extern void showTemperature(int temperature);
extern void showCO2(uint16_t co2);

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
    // should this module get a small independant interface/website?
    request->send(200);
}

void handleLiveStatus(AsyncWebServerRequest *request){
    request->send(200);
}

void handleTimeRequest(AsyncWebServerRequest *request){
    // check if this can fail
    // what should be the fitting response in case of failure?
    showTime();
    request->send(200);
}

void handleJSONRequest(AsyncWebServerRequest *request){
    // apart from a photodiode, this module has no sensors
    // but may get some in the future, idk
    request->send(200);
}

void handleTemperatureRequest(AsyncWebServerRequest *request){
    // almost the same as how i show the weather data
    // request current reading from base
    //getCurrentReading("http://base.local/currentReading", temperature);
    uint8_t successful =  jsonDataRequest("http://base.local/currentReading", 0);
    if(!successful){
        Serial.println("no response or smth");
        request->send(500);
        return;
    }
    // check if reading is valid
    if(doc["sensor"] == "invalid"){
        Serial.println("received invalid reading");
        request->send(500);
        return;
    }
    // i am just assuming the temperature cant be negative because i havent
    // implemented displaying negative numbers
    int temperature = doc["temperature"].as<int>();
    showTemperature(temperature);

    doc.clear();

    //projectString(

    //   either construct string or
    //   projectNumber + 2xprojectCharacter
    //    ° = 65
    // return 200 / some 500 code
    if(request != NULL){
        // i am quietly using this method in main, because i am lazy, so have to
        // check for NULL hehe
        request->send(200);
    }

    request->send(500);
}
void handleCO2Request(AsyncWebServerRequest *request){
    // request current reading from base
    // check if reading is valid
    // if yes
    //  display co2(%4d ppm)
    // else
    //  display "base offline!"
    // return 200 / some 500 code
    if(request != NULL){
        // i am quietly using this method in main, because i am lazy, so have to
        // check for NULL hehe
        request->send(200);
    }
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

// the data is loaded into doc, not sure how i like this implementation, but
// i havent compared different ideas yet
uint8_t jsonDataRequest(const char *path, uint8_t sizeOfData){
   const String* response = httpGetRequest(path);
   DeserializationError err = ArduinoJson::deserializeJson(doc, *(response));

   if(err.Ok){
       Serial.println(err.c_str());
       Serial.println(err.code());
       return 0;
    }
    return 1;
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
