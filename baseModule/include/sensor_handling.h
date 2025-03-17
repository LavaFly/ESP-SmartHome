// Handles Connection to
// -> BME680
// -> Photoresitor
#ifndef SENSOR_HANDLING
#define SENSOR_HANDLING

#include <inttypes.h>
#include <ArduinoJson.h>
#include <SensirionI2cScd30.h>
#include <RTClib.h>


// Maximum Number of concurrent Sensor Readings that will be stored at any time
// the handleJson function fails after 79 elements(will send 79 elements without
// the closing ']')
// will investigate why at some point
#define NUM_READINGS 75




/*
    Simple Implementation of a ring array by storing number of current Readings and start index of the list
*/

unsigned long getSensorTime();
void setSenorTime(unsigned long timeStamp);
void initSensor();
void getSensorReading(char* formattedResponse, size_t maxResponseLen);
void getSensorReadingFromList(char* formattedResponse, size_t maxResponseLen, uint8_t listIndex);


/**
 * @brief
 *
 */
void buildSensorConnection();

/**
 * @brief
 *
 */
bool updateSensorValues();

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
