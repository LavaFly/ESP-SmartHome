#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>

#include "webserver_handling.h"
#include "sensor_handling.h"
#include "internet_settings.h"
#include "time_handling.h"
#include "sd_handling.h"

#define PUMP_POWER 13 // D7

AsyncWebServer server(80);


bool buildRouterConnection(){
    Serial.println("Connecting to WiFi");

    WiFi.begin(APSSID, APPASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        Serial.println("Connected to ap network");
        Serial.println(WiFi.localIP());
        return true;
    }

    WiFi.begin(SSID, PASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        Serial.println("Connected to local network");
        Serial.println(WiFi.localIP());
        return true;
    }

    return false;
}

void initWebserver(){
    if(WiFi.status() != WL_CONNECTED){
        return;
    }
    server.on("/isLive", handleLiveStatus);
    // calling this url will crash the mc, i will fix this someday
    server.on("/currentReading", handleSensorReading);
    server.on("/json", handleJSONRequest);
    server.on("/time", handleTimeRequest);
    server.on("/canvasgz", handleCanvasRequest);


    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "http://base.local");
    server.begin();
    ArduinoOTA.begin();
}

void loopOTA(){
    ArduinoOTA.handle();
}


void setupMDNS(){
    if(!MDNS.begin("tent")){
        Serial.println("Error setting up mDNS responder!");
        while(1){ delay(1000); }
    }
    Serial.println("mDNS responder started");

    // assumes the server has been started, but should be checked for
    MDNS.addService("http", "tcp", 80);
}

void handleLiveStatus(AsyncWebServerRequest *request){
    Serial.println("got liveStatus Request");
    request->send(200);
}

void handleJSONRequest(AsyncWebServerRequest *request){
    Serial.println("got json request");
    int numberOfCurrentReadings = getNumOfReadingsInList();
    char* sensorData = (char*)malloc(sizeof(char) * 180); // rougly 124 will be used
                                                          // check if null

    AsyncResponseStream *response = request->beginResponseStream("application/json");

    response->print("[");
    for(uint8_t i = 0; i < numberOfCurrentReadings; i++){
        getSensorReadingFromList(sensorData, 180, i);
        response->print(sensorData);
        if(i != numberOfCurrentReadings - 1){
            response->print(",");
        }
    }
    response->print("]");
    request->send(response);
    free(sensorData);
}

void handleTimeRequest(AsyncWebServerRequest *request){
    uint32_t unixTimestamp = getEpochTime();

    Serial.println(unixTimestamp);
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", "hello");
    request->send(response);
}
void handleSensorReading(AsyncWebServerRequest *request){
    char* sensorData = (char*)malloc(sizeof(char) * 180); // rougly 124 will be used

    getSensorReading(sensorData, 180);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", sensorData);

    request->send(response);

    free(sensorData);
}

void handleCanvasRequest(AsyncWebServerRequest *request){
    Serial.println("called canvas");
    prepareResponse("canvasjs.min.js.gz"); // this might create a race condition, if another file is being read out while
                                        // one is already open, could use a lock or something, but dont want
                                        // to wait inside the response function, ideally wait until first
                                        // request is done and the resume the other one, see documentation
                                        // as of now, i just ensure that this doesnt happen with a bad workaround
    AsyncWebServerResponse* response = request->beginChunkedResponse("text/javascript", [](uint8_t* buffer, size_t maxLen, size_t index) -> size_t {
        return readFileForResponse(buffer, 256, index);
    });
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
}
