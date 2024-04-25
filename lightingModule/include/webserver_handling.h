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
 */
bool buildRouterConnection();

void setupMDNS();

void lightingOn(AsyncWebServerRequest* request);

void lightingOff(AsyncWebServerRequest* request);

void raiseBrightness(AsyncWebServerRequest* request);

void lowerBrightness(AsyncWebServerRequest* request);

void handleUnkownRequest(AsyncWebServerRequest* request);

void handleLiveStatus(AsyncWebServerRequest *request);

void baseResponse(AsyncWebServerRequest* request);

void initWebserver();

void loopOTA();

extern void sendOffSignal();
extern void sendOnSignal();
extern void sendBrighterSignal();
extern void sendDarkerSignal();

#endif
