// Handles Connection to
// -> DHT22
#ifndef SENSOR_HANDLING
#define SENSOR_HANDLING

#include <inttypes.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <ArduinoJson.h>

#define DHTTYPE DHT22

void initSensor();
void getSensorReading(char* formattedResponse, size_t maxResponseLen);
void printReading();

#endif
