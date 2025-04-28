#include "sensor_handling.h"
#include "time_handling.h"


// Soil Moisture-Pin D7
#define MOISTURE_PIN 13

// Water Level Sensor D5 D6
#define TRIG_PIN 14
#define ECHO_PIN 12

JsonDocument jsonResponse;



/**
 * @brief This structure holds the information of a sensor reading
 *
 */
typedef struct {
    uint32_t time;
    // photo
    float waterLevel;
} sensor_reading;

// this array is used as a ring buffer and holds the sensor information
// readingsListIndex is the index of the newest reading
sensor_reading sensor_readings[NUM_READINGS];
uint8_t readingsListIndex = 0;


uint8_t initSensor(){
    pinMode(MOISTURE_PIN, INPUT);

    // water level
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    return 1;
}


uint8_t getSensorReading(char* formattedResponse, size_t maxResponseLen){
    float distance = 0;
    long duration = 0;


    // it is a bad idea to use delays like this in combination with some of the
    // other libraries im using, but hoping this wont cause a mistake
    yield();
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    yield();

    distance = duration * DISTANCE_CONVERSION;


    // this will be usefull, once i add more sensors
    if(true){
        jsonResponse["sensor"] = "grow";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["waterLevel"] = distance;

    } else {
        jsonResponse["sensor"] = "invalid";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["waterLevel"] = 0;
    }

    jsonResponse.shrinkToFit();
    serializeJson(jsonResponse, formattedResponse, maxResponseLen);
    return 1;
}

uint8_t getSensorReadingFromList(char* formattedResponse, size_t maxResponseLen, uint8_t listIndex){
    // index calculation
    uint8_t numberOfReadings = getNumOfReadingsInList();
    uint8_t temp = (listIndex + readingsListIndex) % numberOfReadings;

    jsonResponse["sensor"] = "grow";
    jsonResponse["time"] = sensor_readings[temp].time;
    jsonResponse["waterLevel"] = sensor_readings[temp].waterLevel;

    jsonResponse.shrinkToFit();
    serializeJson(jsonResponse, formattedResponse, maxResponseLen);
    return 1;
}

uint8_t getNumOfReadingsInList(){
    for(uint8_t numberOfReadings = 0; numberOfReadings < NUM_READINGS; numberOfReadings++){
        if(sensor_readings[numberOfReadings].time < 1) return numberOfReadings;
    }
    return NUM_READINGS;
}

uint8_t updateSensorValues(){
    float distance;
    long duration = 0;


    // it is a bad idea to use delays like this in combination with some of the
    // other libraries im using, but hoping this wont cause a mistake
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);

    distance = duration * DISTANCE_CONVERSION;


    sensor_readings[readingsListIndex].time = getEpochTime();
    sensor_readings[readingsListIndex].waterLevel = distance;

    readingsListIndex = (readingsListIndex + 1) % NUM_READINGS;
    return 1;
}

void printCurrentReading(){
    Serial.println("Not implemented");
}
