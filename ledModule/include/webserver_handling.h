#ifndef WEBSERVER
#define WEBSERVER

// Manages the local Webserver
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

/**
 * @brief
 *
 */
bool buildRouterConnection();

/**
 * @brief
 *
 */


bool initWebserver();

void setupMDNS();

void loopOTA();


/**
 * @brief Serves the Client the gzipped webpage from the FLASH Memory
 *  Client will afterwards call handleJSONRequest() to fill the Chart with data
 *
 */
void handleHTMLRequest(AsyncWebServerRequest*);

void handleLiveStatus(AsyncWebServerRequest *request);

void handleTimeRequest(AsyncWebServerRequest *request);
void handleTemperatureRequest(AsyncWebServerRequest *request);
void handleQualityRequest(AsyncWebServerRequest *request);
void handleCO2Request(AsyncWebServerRequest *request);
/**
 * @brief Serves the Sensor Readings Array as an JSON structure
 * JSON structure is manually constructed without the use of the ArduinoJSON Library
 *
 */
void handleJSONRequest(AsyncWebServerRequest*);

int httpGetRequestIgnoreResponse(const char* path);

const String* httpGetRequest(const char* path);

WiFiClient& httpGetRequestStream(const char* path);

void httpEndRequestStream();

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);

void cleanUpSockets();

#endif
