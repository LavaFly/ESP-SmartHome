// Handles Connection to
// -> DHT22
#ifndef SENSOR_HANDLING
#define SENSOR_HANDLING

#include <inttypes.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

#define DHTTYPE DHT22

void initSensor();
float getSensorReading();
void printReading();

#endif
