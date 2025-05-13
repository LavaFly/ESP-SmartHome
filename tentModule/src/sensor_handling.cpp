#include "sensor_handling.h"
#include "time_handling.h"

// Pin for co2 Measurement
#define     PWM_PIN 16

JsonDocument jsonResponse;
Adafruit_BME680 bme;

/**
 * @brief This structure holds the information of a sensor reading
 *
 */
typedef struct {
    uint32_t time;
    float temperature;
    float humidity;
    float resistance;

    uint16_t brightness;
    uint8_t co2;
} sensor_reading;

// this array is used as a ring buffer and holds the sensor information
// readingsListIndex is the index of the newest reading
sensor_reading sensor_readings[NUM_READINGS];
uint8_t readingsListIndex = 0;


uint8_t initSensor(){
    // both brightness and co2 sensor connections cant be confirmed
    // so only the bme680 is relevant
    pinMode(PWM_PIN, INPUT);

    for(uint8_t i = 0; i < 200; i++){
        if(bme.begin(119)){
            bme.setTemperatureOversampling(BME680_OS_8X);
            bme.setHumidityOversampling(BME680_OS_2X);
            bme.setPressureOversampling(BME680_OS_4X);
            bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
            bme.setGasHeater(320, 150);

            return 1;
        }
        delay(50);
    }
    Serial.println("fuck");

    return 0;
}


uint8_t getSensorReading(char* formattedResponse, size_t maxResponseLen){
    // calculation for co2
    float pulsepercent;
    uint32_t co2;
    uint32_t pwmtime;
    pwmtime = pulseIn(PWM_PIN, HIGH, 2000000) / 1000;
    pulsepercent = pwmtime / 1004.0;
    co2 = 5000 * pulsepercent;

    uint8_t readingSuccessful = false;

    if(bme.performReading()){
        jsonResponse["sensor"] = "tent";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = bme.temperature;
        jsonResponse["humidity"] = bme.humidity;
        jsonResponse["resistance"] = bme.gas_resistance;
        jsonResponse["brightness"] = analogRead(A0);
        jsonResponse["co2"] = co2;
        readingSuccessful = true;
    } else {
        jsonResponse["sensor"] = "invalid";
        jsonResponse["time"] = 0;
        jsonResponse["temperature"] = 0;
        jsonResponse["humidity"] = 0;
        jsonResponse["resistance"] = 0;
        jsonResponse["brightness"] = 0;
        jsonResponse["co2"] = 0;
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

    jsonResponse["sensor"] = "tent";
    jsonResponse["time"] = sensor_readings[temp].time;
    jsonResponse["temperature"] = sensor_readings[temp].temperature;
    jsonResponse["humidity"] = sensor_readings[temp].humidity;
    jsonResponse["brightness"] = sensor_readings[temp].brightness;
    jsonResponse["co2"] = sensor_readings[temp].co2;
    jsonResponse["resistance"] = sensor_readings[temp].resistance;

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
    float pulsepercent;
    uint32_t co2;
    unsigned long pwmtime;
    pwmtime = pulseIn(PWM_PIN, HIGH, 2000000) / 1000;
    pulsepercent = pwmtime / 1004.0;
    co2 = 5000 * pulsepercent;

    if(!bme.performReading()){
        return 0;
    }

    sensor_readings[readingsListIndex].time = getEpochTime();
    sensor_readings[readingsListIndex].temperature = bme.temperature;
    sensor_readings[readingsListIndex].humidity = bme.humidity;
    sensor_readings[readingsListIndex].resistance = bme.gas_resistance;
    sensor_readings[readingsListIndex].brightness = analogRead(A0);
    sensor_readings[readingsListIndex].co2 = co2;

    readingsListIndex = (readingsListIndex + 1) % NUM_READINGS;
    return 1;
}

void printCurrentReading(){
    Serial.println("Not implemented");
}
