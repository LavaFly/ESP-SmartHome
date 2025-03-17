#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "ESPAsyncWebServer.h"
#include "WebResponseImpl.h"
#include <ArduinoOTA.h>

#include "webserver_handling.h"
#include "sensor_handling.h"
#include "internet_settings.h"
#include "time_handling.h"

#include "webpage_html.h"
#include "webpage_js.h"
#include "webpage_css.h"

AsyncWebServer server(80);
AsyncUDP udp;
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

void setupNTPServer(){
    WiFi.disconnect();

    WiFi.begin("testNetwork", "thisisatest");
    WiFi.waitForConnectResult();

    if(udp.listen(123)){
        Serial.println("udp time request");
        udp.onPacket(handleTimeRequest);
    }
    Serial.print("IP: ");
    Serial.println(WiFi.softAPIP());
    /**

    // Connect to local network
    WiFi.begin(SSID, PASS);
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
        Serial.println("Connected to local network");
        Serial.println(WiFi.localIP());
        uint32_t timeStamp = getEpochTime();
        setSenorTime(timeStamp),
        Serial.println("Starting AP-Mode");
        //WiFi.softAPConfig(local_IP, gateway, subnet);
        //WiFi.softAP(APSSID, APPASS, 1, 7);

    } else {
        Serial.println("fuck");
    }

    **/


    // Get current time
    // Set rtc time
    // disconnect from loca network and setup own ap
}

void handleTimeRequest(AsyncUDPPacket &packet){
    // Taken from and adapted from
    // https://github.com/Willtech/NTP-Server-for-ESP8266


    // Output IP of NTP request
    Serial.print("Received NTP request from IP: ");
    Serial.println(packet.remoteIP());

    // Import packet
    byte packetBuffer[48];
    memcpy(packetBuffer, packet.data(), 48);
    //packet.write(packetBuffer, 48);


    Serial.println("Received UDP packet:");
    for (int i = 0; i < 48; i++) {
        Serial.print(packetBuffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    // Get current RTC DS3231 time
    unsigned long currentTime = getSensorTime();
    Serial.print("currTime = ");
    Serial.println(currentTime);
    unsigned long secsSince1900 = currentTime + 2208988800UL - 0;
    Serial.print("secs = ");
    Serial.println(secsSince1900);

    // Prepare the response packet
    packetBuffer[0] = 0b00100100;  // LI, Version, Mode
    packetBuffer[1] = 1;           // Stratum, or type of clock
    packetBuffer[2] = 0;           // Polling Interval
    packetBuffer[3] = 0xEC;        // Peer Clock Precision
    memset(packetBuffer + 4, 0, 8); // Root Delay & Root Dispersion
    memset(packetBuffer + 12, 0, 4); // Reference Identifier

    // Reference Timestamp
    packetBuffer[16] = (byte)(secsSince1900 >> 24);
    packetBuffer[17] = (byte)(secsSince1900 >> 16);
    packetBuffer[18] = (byte)(secsSince1900 >> 8);
    packetBuffer[19] = (byte)(secsSince1900);
    memset(packetBuffer + 20, 0, 4);

    // Originate Timestamp (copied from request)
    memcpy(packetBuffer + 24, packetBuffer + 40, 8);

    // Receive Timestamp
    packetBuffer[32] = (byte)(secsSince1900 >> 24);
    packetBuffer[33] = (byte)(secsSince1900 >> 16);
    packetBuffer[34] = (byte)(secsSince1900 >> 8);
    packetBuffer[35] = (byte)(secsSince1900);
    memset(packetBuffer + 36, 0, 4);

    // Transmit Timestamp
    packetBuffer[40] = (byte)(secsSince1900 >> 24);
    packetBuffer[41] = (byte)(secsSince1900 >> 16);
    packetBuffer[42] = (byte)(secsSince1900 >> 8);
    packetBuffer[43] = (byte)(secsSince1900);
    memset(packetBuffer + 44, 0, 4);

    // Print the response packet for debugging
    Serial.println("Response UDP packet:");
    for (int i = 0; i < 48; i++) {
    Serial.print(packetBuffer[i], HEX);
    Serial.print(" ");
    }
    Serial.println();

    //uint8_t a = udp.write(packetBuffer, 48);
    AsyncUDPMessage ans(48);
    ans.write(packetBuffer, 48);
    packet.send(ans);


}


void initWebserver(){
    if(WiFi.status() != WL_CONNECTED){
        return;
    }
    server.on("/", handleHTMLRequest);
    server.on("/js", handleJSRequest);
    server.on("/css", handleCSSRequest);
    server.on("/isLive", handleLiveStatus);
    server.on("/json", handleJSONRequest);
    server.on("/currentReading", handleSensorReading);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    server.begin();
    ArduinoOTA.begin();
}

void loopOTA(){
    ArduinoOTA.handle();
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

void handleLiveStatus(AsyncWebServerRequest *request){
    Serial.println("got liveStatus Request");
    request->send(200);
}

void handleHTMLRequest(AsyncWebServerRequest *request){
    const char* dataType = "text/html";
    AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, webpage_html_gz, webpage_html_gz_len);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
}

void handleJSRequest(AsyncWebServerRequest *request){
    const char* dataType = "text/javascript";
    AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, webpage_js_gz, webpage_js_gz_len);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);

}


void handleCSSRequest(AsyncWebServerRequest *request){
    const char* dataType = "text/css";
    AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, webpage_css_gz, webpage_css_gz_len);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);

}


void handleJSONRequest(AsyncWebServerRequest *request){
    Serial.println("got json request");
    int numberOfCurrentReadings = getNumOfReadingsInList();
    char* sensorData = (char*)malloc(sizeof(char) * 180); // rougly 124 will be used
                                                          // check if null!

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
void handleSensorReading(AsyncWebServerRequest *request){
    char* sensorData = (char*)malloc(sizeof(char) * 180); // rougly 124 will be used

    getSensorReading(sensorData, 180);

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
