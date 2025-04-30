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
 * @param
 * 
 * @return uint8_t
 */
uint8_t buildRouterConnection();


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
uint8_t buildAP();


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
uint8_t buildNTPServer();


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
uint8_t initWebserver();


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
uint8_t setupMDNS();


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
void loopOTA();


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
void handleTimeRequest(AsyncUDPPacket &packet);


/**
 * @brief Serves the Client the gzipped webpage from the FLASH Memory
 *  Client will afterwards call handleJSONRequest() to fill the Chart with data
 * 
 * @param
 * 
 * @return uint8_t
 */
void handleHTMLRequest(AsyncWebServerRequest *request);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
void handleSensorReading(AsyncWebServerRequest *request);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
void handleLiveStatus(AsyncWebServerRequest *request);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
void handleTimeStringRequest(AsyncWebServerRequest *request);


/**
 * @brief Serves the Sensor Readings Array as an JSON structure
 * JSON structure is manually constructed without the use of the ArduinoJSON Library
 * 
 * @param
 * 
 * @return uint8_t
 */
void handleJSONRequest(AsyncWebServerRequest*);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
void handleJSRequest(AsyncWebServerRequest*);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
void handleCSSRequest(AsyncWebServerRequest*);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
void handleCanvasRequest(AsyncWebServerRequest*);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
void handleJQueryRequest(AsyncWebServerRequest*);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
uint8_t httpGetRequestIgnoreResponse(const char* path);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
const String* httpGetRequest(const char* path);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
WiFiClient& httpGetRequestStream(const char* path);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
void httpEndRequestStream();

#endif