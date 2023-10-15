#ifndef WEBSERVER
#define WEBSERVER

// Manages the local Webserver
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <time.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


/**
 * @brief 
 * 
 */
void buildRouterConnection();

/**
 * @brief 
 * 
 */
void buildTimeServerConnection(); // find fitting location for this method

/**
 * @brief Serves the Client the gzipped webpage from the FLASH Memory 
 *  Client will afterwards call handleJSONRequest() to fill the Chart with data
 * 
 */
void handleHTMLRequest(AsyncWebServerRequest*);

/**
 * @brief Serves the Sensor Readings Array as an JSON structure
 * JSON structure is manually constructed without the use of the ArduinoJSON Library
 * 
 */
void handleJSONRequest(AsyncWebServerRequest*);

#endif