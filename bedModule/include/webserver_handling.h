#ifndef WEBSERVER
#define WEBSERVER

// Manages the local Webserver
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>

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


/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
void lightingOn(AsyncWebServerRequest* request);


/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
void lightingOff(AsyncWebServerRequest* request);


/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
void raiseBrightness(AsyncWebServerRequest* request);


/**
 * @brief
 *
 * @param
 *
 * @return uint8_t
 */
void lowerBrightness(AsyncWebServerRequest* request);


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

#endif
