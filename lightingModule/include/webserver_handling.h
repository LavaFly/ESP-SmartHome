#ifndef WEBSERVER
#define WEBSERVER

// Manages the local Webserver
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

extern uint8_t lightingBrightness;
extern bool lightingStatus;

/**
 * @brief
 *
 */
bool buildRouterConnection();

void lightingOn(AsyncWebServerRequest* request);

void lightingOff(AsyncWebServerRequest* request);

void raiseBrightness(AsyncWebServerRequest* request);

void lowerBrightness(AsyncWebServerRequest* request);

void handleUnkownRequest(AsyncWebServerRequest* request);

void initWebserver();

#endif
