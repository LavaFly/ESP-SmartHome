#ifndef WEBSERVER
#define WEBSERVER

// Manages the local Webserver
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>


/**
 * @brief
 *
 */
bool buildRouterConnection();

void initWebserver();

void setupMDNS();

void loopOTA();



/**
 * @brief Serves the Sensor Readings Array as an JSON structure
 * JSON structure is manually constructed without the use of the ArduinoJSON Library
 *
 */
void handleJSONRequest(AsyncWebServerRequest*);

void handleSensorReading(AsyncWebServerRequest *request);

void handleLiveStatus(AsyncWebServerRequest *request);
void handleTimeRequest(AsyncWebServerRequest *request);
void handleCanvasRequest(AsyncWebServerRequest *request);

extern void getSensorReading(char *formattedResponse, size_t maxResponseLen);
extern void activateActuator();
extern void deactivateActuator();

#endif
