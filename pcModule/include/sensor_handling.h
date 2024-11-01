// Handles Connection to
// -> DHT22
#ifndef SENSOR_HANDLING
#define SENSOR_HANDLING

#include <inttypes.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"


// Maximum Number of concurrent Sensor Readings that will be stored at any time
// the handleJson function fails after 79 elements(will send 79 elements without
// the closing ']')
// will investigate why at some point
#define NUM_READINGS 60

#define DHTTYPE DHT22

#define STATUSPIN 0
#define POWERPIN 5

void initSensor();
void getSensorReading(char* formattedResponse, size_t maxResponseLen);
void getSensorReadingFromList(char* formattedResponse, size_t maxResponseLen, uint8_t listIndex);
float getTemp();
float getHumid();

void buildSensorConnection();
bool updateSensorValues();
int addReadingToList();
int getNumOfReadingsInList();
int readAtIndex();
void printCurrentReading();

#endif
