#ifndef SENSOR_HANDLING
#define SENSOR_HANDLING

#include <inttypes.h>
#include <ArduinoJson.h>
#include "Adafruit_BME680.h"

#define NUM_READINGS 120
#define STATUS_PIN 13
#define DHTTYPE DHT11


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
uint8_t initSensor();


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
uint8_t getSensorReading(char* formattedResponse, size_t maxResponseLen);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
uint8_t getSensorReadingFromList(char* formattedResponse, size_t maxResponseLen, uint8_t listIndex);


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
uint8_t buildSensorConnection();


/**
 * @brief 
 * 
 * @param
 * 
 * @return uint8_t
 */
uint8_t updateSensorValues();


/**
 * @brief Get the Num Of Readings In List object
 * 
 * @param
 * 
 * @return uint8_t
 */
uint8_t getNumOfReadingsInList();


/**
 * @brief Used for debugging, prints current values the Sensor sends back
 * 
 * @param
 */
void printCurrentReading();

#endif