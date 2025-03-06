#include "sensor_handling.h"
#include "time_handling.h"

JsonDocument jsonResponse;
Adafruit_BME680 bme;

uint8_t readingsListIndex = 0;


// this ugly
#define PWM_PIN 15
const int ppmrange = 5000;


/**
 * @brief This Structure holds all information that is stored from a sensor reading
 *
 */
typedef struct {
    uint32_t time;
    float temperature;
    float humidity;
    float quality;

    uint16_t brightness;
    uint8_t co2;
} sensor_reading;
sensor_reading sensor_readings[NUM_READINGS];

void initSensor(){
    Serial.println("start init");
    pinMode(PWM_PIN, INPUT);

    while(!bme.begin(119)){
        Serial.print(".");
        delay(50);
    }
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150);

    analogRead(A0);
    Serial.println("Sensor setup done");
}


void getSensorReading(char* formattedResponse, size_t maxResponseLen){
    float pulsepercent;
    uint32_t co2;
    unsigned long pwmtime;
    pwmtime = pulseIn(PWM_PIN, HIGH, 2000000) / 1000;
    pulsepercent = pwmtime / 1004.0;
    co2 = ppmrange * pulsepercent;

    // do this prettier at some point
    if(bme.performReading()){
        jsonResponse["sensor"] = "tent";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = bme.temperature;
        jsonResponse["humidity"] = bme.humidity;
        jsonResponse["quality"] = bme.gas_resistance;
        jsonResponse["brightness"] = analogRead(A0);
        jsonResponse["co2"] = co2;

    } else {
        // i should probably print or log this
        jsonResponse["sensor"] = "invalid";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = 0;
        jsonResponse["humidity"] = 0;
        jsonResponse["quality"] = 0;
        jsonResponse["brightness"] = 0;
        jsonResponse["co2"] = 0;
    }

    jsonResponse.shrinkToFit();
    serializeJson(jsonResponse, formattedResponse, maxResponseLen);
}

void getSensorReadingFromList(char* formattedResponse, size_t maxResponseLen, uint8_t listIndex){
    // index calculation
    uint8_t numberOfReadings = getNumOfReadingsInList();
    uint8_t temp = (listIndex + readingsListIndex) % numberOfReadings;

    jsonResponse["sensor"] = "base";
    jsonResponse["time"] = sensor_readings[temp].time;
    jsonResponse["temperature"] = sensor_readings[temp].temperature;
    jsonResponse["humidity"] = sensor_readings[temp].humidity;
    jsonResponse["brightness"] = sensor_readings[temp].brightness;
    jsonResponse["co2"] = sensor_readings[temp].co2;

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
    float pulsepercent;
    uint32_t co2;
    unsigned long pwmtime;
    pwmtime = pulseIn(PWM_PIN, HIGH, 2000000) / 1000;
    pulsepercent = pwmtime / 1004.0;
    co2 = ppmrange * pulsepercent;

    if(!bme.performReading()){
        return false;
    }

    sensor_readings[readingsListIndex].time = getEpochTime();
    Serial.println(sensor_readings[readingsListIndex].time);
    Serial.println("");
    sensor_readings[readingsListIndex].temperature = bme.temperature;
    sensor_readings[readingsListIndex].humidity = bme.humidity;
    sensor_readings[readingsListIndex].quality = bme.gas_resistance;
    sensor_readings[readingsListIndex].brightness = analogRead(A0);
    sensor_readings[readingsListIndex].co2 = co2;

    readingsListIndex = (readingsListIndex + 1) % NUM_READINGS;
    return true;
}

void printCurrentReading(){
    Serial.println("New Reading");
    Serial.print("brightness ");
    Serial.println(analogRead(A0));
    Serial.println("\n");
}
