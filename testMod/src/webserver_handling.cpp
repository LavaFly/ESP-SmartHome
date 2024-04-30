#include "webserver_handling.h"
#include "ESPAsyncWebServer.h"
#include "WebResponseImpl.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ElegantOTA.h>
#include "sensor_handling.h"
#include "mock_sensor.h"
#include "internet_settings.h"
#include "webpage.h"


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
    ElegantOTA.begin(&server);
    server.on("/", handleHTMLRequest);
    server.on("/sensorReading", handleSensorReading);
    server.on("/allData", handleJSONRequest);
    server.begin();
}

void loopOTA(){
    ElegantOTA.loop();
}


void setupMDNS(){
    if(!MDNS.begin("baseModule")){
        Serial.println("Error setting up mDNS responder!");
        while(1){ delay(1000); }
    }
    Serial.println("mDNS responder stbarted");

    // assumes the server has been started, but should be checked for
    MDNS.addService("http", "tcp", 80);
}


void handleHTMLRequest(AsyncWebServerRequest *request){
    const char* dataType = "text/html";
    //Serial.println("Streaming Page!");
    AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, webpage_html_gz, webpage_gz_len);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
}

void handleJSONRequest(AsyncWebServerRequest *request){
    const char* dataType = "text/html";
    //Serial.println("Streaming Page!");
    AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, mock_html_gz, mock_gz_len);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);

    /**
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
    **/
}
void handleSensorReading(AsyncWebServerRequest *request){
    char* sensorData = (char*)malloc(sizeof(char) * 180); // rougly 124 will be used

    //getSensorReading(sensorData, 180);

    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", sensorData);

    request->send(response);

    free(sensorData);
}

int httpGetRequestIgnoreResponse(const char* path){
    if(http.begin(client, path)){
        int httpCode = http.GET();
        if(httpCode == HTTP_CODE_OK){
            return 1;
        }
        http.end();
    }
    return 0;
}