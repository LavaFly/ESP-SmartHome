#include <ArduinoJson.h>
#include "sensor_handling.h"
#include "time_handling.h"


JsonDocument jsonResponse;
DHT dht(0, DHTTYPE);


/**
 * @brief This structure holds the information of a sensor reading
 *
 */
typedef struct {
    uint32_t time;
    // dht22
    float temperature;
    float humidity;
    // add pcStatus?
} sensor_reading;

// this array is used as a ring buffer and holds the sensor information
// readingsListIndex is the index of the newest reading
sensor_reading sensor_readings[NUM_READINGS];
uint8_t readingsListIndex = 0;


uint8_t initSensor(){
    // this shouldnt really be here, oh well
    pinMode(POWERPIN, OUTPUT);

    // check if i can confirm the connection somehow
    dht.begin();
    return 1;
}


uint8_t getSensorReading(char* formattedResponse, size_t maxResponseLen){
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    uint8_t readingSuccessful = false;

    if(isnan(humidity) || isnan(temperature)){
        jsonResponse["sensor"] = "invalid";
        jsonResponse["time"] = 0;
        jsonResponse["temperature"] = 0;
        jsonResponse["humidity"] = 0;
    } else {
        jsonResponse["sensor"] = "pc";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = temperature;
        jsonResponse["humidity"] = humidity;
        readingSuccessful = true;

    }
    jsonResponse.shrinkToFit();
    serializeJson(jsonResponse, formattedResponse, maxResponseLen);
    return readingSuccessful;
}


uint8_t getSensorReadingFromList(char* formattedResponse, size_t maxResponseLen, uint8_t listIndex){
    // index calculation
    uint8_t numberOfReadings = getNumOfReadingsInList();
    uint8_t temp = (listIndex + readingsListIndex) % numberOfReadings;

    uint8_t validReading = false;
    if(sensor_readings[temp].time != 0){
        validReading = true;
    }

    jsonResponse["sensor"] = "pc";
    jsonResponse["time"] = sensor_readings[temp].time;
    jsonResponse["temperature"] = sensor_readings[temp].temperature;
    jsonResponse["humidity"] = sensor_readings[temp].humidity;

    jsonResponse.shrinkToFit();
    serializeJson(jsonResponse, formattedResponse, maxResponseLen);
    return validReading;
}


uint8_t getNumOfReadingsInList(){
    for(uint8_t numberOfReadings = 0; numberOfReadings < NUM_READINGS; numberOfReadings++){
        if(sensor_readings[numberOfReadings].time < 1) return numberOfReadings;
    }
    return NUM_READINGS;
}


uint8_t updateSensorValues(){
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // if reading values from the sensor fails, return false, else store values and return true
    if(isnan(humidity) || isnan(temperature)){
        return 0;
    }

    sensor_readings[readingsListIndex].time = getEpochTime();
    sensor_readings[readingsListIndex].temperature = temperature;
    sensor_readings[readingsListIndex].humidity = humidity;

    readingsListIndex = (readingsListIndex + 1) % NUM_READINGS;
    return 1;
}


void printCurrentReading(){
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    Serial.print("temperature ");
    Serial.println(temperature);
    Serial.print("humidity ");
    Serial.println(humidity);
    Serial.println();
}