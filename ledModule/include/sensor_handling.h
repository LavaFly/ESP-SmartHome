// Handles Connection to 
// -> BME680
// -> Photoresitor
#ifndef SENSOR_HANDLING
#define SENSOR_HANDLING

#include <inttypes.h>

// Maximum Number of concurrent Sensor Readings that will be stored at any time
#define NUM_READINGS 128

/**
 * @brief This Structure holds all information that is stored from a sensor reading
 *  ( the BME680 is capable of reading airpressure, though this is practically unchanging in my application and thus will not be stored )
 * 
 */
/*
struct sensor_reading{
    uint32_t time;
    float temperature;
    float humidity;
    float quality;
    uint16_t brightness;
} sensor_readings[NUM_READINGS];*/



/*
    Simple Implementation of a ring array by storing number of current Readings and start index of the list
*/


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