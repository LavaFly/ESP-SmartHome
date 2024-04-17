// Handles Connection to
// -> BME680
// -> Photoresitor
#ifndef SENSOR_HANDLING
#define SENSOR_HANDLING

#include <inttypes.h>
#include <ArduinoJson.h>
#include "Adafruit_BME680.h"


// Maximum Number of concurrent Sensor Readings that will be stored at any time
#define NUM_READINGS 128

/**
 * @brief This Structure holds all information that is stored from a sensor reading
 *  ( the BME680 is capable of reading airpressure, though this is practically unchanging in my application and thus will not be stored )
 *
 */


/*
    Simple Implementation of a ring array by storing number of current Readings and start index of the list
*/

void initSensor();
void getSensorReading(char* formattedResponse, size_t maxResponseLen);
void getSensorReadingFromList(char* formattedResponse, size_t maxResponseLen, uint8_t listIndex);




// add new reading to list
// add new reading at index
void updateSensorValues();


/**
 * @brief
 *
 */
void buildSensorConnection();

/**
 * @brief
 *
 */
void updateReadingList();

/**
 * @brief
 *
 * @return int
 */
int addReadingToList();

/**
 * @brief Get the Num Of Readings In List object
 *
 * @return int
 */
int getNumOfReadingsInList();

/**
 * @brief
 *
 * @return int
 */
int readAtIndex();

/**
 * @brief Used for debugging, prints current values the Sensor sends back
 *
 */
void printCurrentReading();

#endif
