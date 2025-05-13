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
 * @brief Serves the Client the gzipped webpage from the FLASH Memory
 *  Client will afterwards call handleJSONRequest() to fill the Chart with data
 *
 */
/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
void handleHTMLRequest(AsyncWebServerRequest*);


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
void handleTimeRequest(AsyncWebServerRequest *request);


/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
void handleTemperatureRequest(AsyncWebServerRequest *request);


/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
void handleQualityRequest(AsyncWebServerRequest *request);


/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
void handleCO2Request(AsyncWebServerRequest *request);


/**
 * @brief Serves the Sensor Readings Array as an JSON structure
 * JSON structure is manually constructed without the use of the ArduinoJSON Library
 *
 */
/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
void handleJSONRequest(AsyncWebServerRequest*);

/**
 * @brief
 *
 */
/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
uint8_t jsonDataRequest(const char *path, uint8_t sizeOfData);


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


/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);


/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
uint8_t cleanUpSockets();

#endif
