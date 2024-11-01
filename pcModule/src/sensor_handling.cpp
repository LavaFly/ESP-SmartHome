#include <ArduinoJson.h>
#include "sensor_handling.h"
#include "time_handling.h"


JsonDocument jsonResponse;
DHT dht(0, DHTTYPE);


uint8_t readingsListIndex = 0;

/**
 * @brief This Structure holds all information that is stored from a sensor reading
 *
 */
typedef struct {
    uint32_t time;
    // dht22
    float temperature;
    float humidity;
    // add pcStatus?
} sensor_reading;
sensor_reading sensor_readings[NUM_READINGS];


void initSensor(){

    pinMode(POWERPIN, OUTPUT);
    dht.begin();

    Serial.println("Sensor setup done");
}

void getSensorReading(char* formattedResponse, size_t maxResponseLen){
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if(isnan(humidity) || isnan(temperature)){
        jsonResponse["sensor"] = "invalid";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = 0;
        jsonResponse["humidity"] = 0;
    } else {
        jsonResponse["sensor"] = "dht22";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = temperature;
        jsonResponse["humidity"] = humidity;
    }
    jsonResponse.shrinkToFit();
    serializeJson(jsonResponse, formattedResponse, maxResponseLen);
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


void getSensorReadingFromList(char* formattedResponse, size_t maxResponseLen, uint8_t listIndex){
    // index calculation
    uint8_t numberOfReadings = getNumOfReadingsInList();
    uint8_t temp = (listIndex + readingsListIndex) % numberOfReadings;
    jsonResponse["sensor"] = "dht22";
    jsonResponse["time"] = sensor_readings[temp].time;
    jsonResponse["temperature"] = sensor_readings[temp].temperature;
    jsonResponse["humidity"] = sensor_readings[temp].humidity;

    jsonResponse.shrinkToFit();
    serializeJson(jsonResponse, formattedResponse, maxResponseLen);
}

int getNumOfReadingsInList(){
    for(uint8_t numberOfReadings = 0; numberOfReadings < NUM_READINGS; numberOfReadings++){
        if(sensor_readings[numberOfReadings].time < 1) return numberOfReadings;
    }
    return NUM_READINGS;
}


bool updateSensorValues(){
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // if reading values from the sensor fails, return false, else store values and return true
    if(isnan(humidity) || isnan(temperature)){
        return false;
    }

    sensor_readings[readingsListIndex].time = getEpochTime();
    sensor_readings[readingsListIndex].temperature = temperature;
    sensor_readings[readingsListIndex].humidity = humidity;

    readingsListIndex = (readingsListIndex + 1) % NUM_READINGS;
    return true;
}
