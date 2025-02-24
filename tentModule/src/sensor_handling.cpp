#include "sensor_handling.h"
#include "time_handling.h"

JsonDocument jsonResponse;
uint8_t readingsListIndex = 0;
DHT dht(STATUS_PIN, DHTTYPE);

// this ugly
#define PWM_PIN 15
const int ppmrange = 5000;

/**
 * @brief This Structure holds all information that is stored from a sensor reading
 *
 */
typedef struct {
    uint32_t time;
    // dht11
    float temperature;
    float humidity;
    uint16_t brightness;
    uint8_t co2;
} sensor_reading;
sensor_reading sensor_readings[NUM_READINGS];

void initSensor(){
    Serial.println("start init");
    pinMode(PWM_PIN, INPUT);

    dht.begin();

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

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // do this prettier at some point
    if(!isnan(temperature) || !isnan(humidity)){
        jsonResponse["sensor"] = "tent";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = temperature;
        jsonResponse["humidity"] = humidity;
        jsonResponse["brightness"] = analogRead(A0);
        jsonResponse["co2"] = co2;

    } else {
        // i should probably print or log this
        jsonResponse["sensor"] = "invalid";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = 0;
        jsonResponse["humidity"] = 0;
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

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if(isnan(temperature) || isnan(humidity)){
        return false;
    }

    sensor_readings[readingsListIndex].time = getEpochTime();
    Serial.println(sensor_readings[readingsListIndex].time);
    Serial.println("");
    sensor_readings[readingsListIndex].temperature = temperature;
    sensor_readings[readingsListIndex].humidity = humidity;
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
