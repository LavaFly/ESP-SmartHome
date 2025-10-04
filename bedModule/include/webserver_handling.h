#ifndef WEBSERVER
#define WEBSERVER

// Manages the local Webserver
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsClient.h>

extern uint8_t lightingBrightness;
extern bool lightingStatus;

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

uint8_t httpGetRequestIgnoreResponse(const char* path);

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
void handleUnkownRequest(AsyncWebServerRequest* request);


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
void baseResponse(AsyncWebServerRequest* request);

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);

#endif
