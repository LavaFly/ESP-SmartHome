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
void handleTimeRequest(AsyncWebServerRequest *request);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
void handleCanvasRequest(AsyncWebServerRequest *request);


extern uint8_t getSensorReading(char *formattedResponse, size_t maxResponseLen);
extern void activateActuator();
extern void deactivateActuator();

#endif
