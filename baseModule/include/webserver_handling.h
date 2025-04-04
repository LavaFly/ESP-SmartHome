#ifndef WEBSERVER
#define WEBSERVER

// Manages the local Webserver
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncUDP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


/**
 * @brief
 *
 */
bool buildRouterConnection();

bool buildAP();

bool buildNTPServer();

bool initWebserver();

void setupMDNS();

void loopOTA();

void handleTimeRequest(AsyncUDPPacket &packet);

/**
 * @brief Serves the Client the gzipped webpage from the FLASH Memory
 *  Client will afterwards call handleJSONRequest() to fill the Chart with data
 *
 */
void handleHTMLRequest(AsyncWebServerRequest *request);


void handleSensorReading(AsyncWebServerRequest *request);

void handleLiveStatus(AsyncWebServerRequest *request);



/**
 * @brief Serves the Sensor Readings Array as an JSON structure
 * JSON structure is manually constructed without the use of the ArduinoJSON Library
 *
 */
void handleJSONRequest(AsyncWebServerRequest*);

void handleJSRequest(AsyncWebServerRequest*);
void handleCSSRequest(AsyncWebServerRequest*);

void handleCanvasRequest(AsyncWebServerRequest*);
void handleJQueryRequest(AsyncWebServerRequest*);

int httpGetRequestIgnoreResponse(const char* path);

const String* httpGetRequest(const char* path);

WiFiClient& httpGetRequestStream(const char* path);

void httpEndRequestStream();

#endif
