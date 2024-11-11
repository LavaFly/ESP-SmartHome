#include "sensor_handling.h"
#include "time_handling.h"

JsonDocument jsonResponse;
DHT dht(STATUSPIN, DHTTYPE);

uint8_t readingsListIndex = 0;

// Photoresistor-Pin D8
#define BRIGHTNESS_PIN 15

// Soil Moisture-Pin D7
#define MOISTURE_PIN 13

// Water Level Sensor D5 D6
#define TRIG_PIN 14
#define ECHO_PIN 12

/**
 * @brief This Structure holds all information that is stored from a sensor reading
 *
 */
typedef struct {
    uint32_t time;
    // dht22
    float temperature;
    float humidity;
    // photo
    uint16_t brightness;
    uint16_t waterLevel;
} sensor_reading;
sensor_reading sensor_readings[NUM_READINGS];

void initSensor(){
    Serial.println("start init");
    pinMode(BRIGHTNESS_PIN, INPUT);
    pinMode(MOISTURE_PIN, INPUT);

    dht.begin();

    // init analog sensors
    // set brightness pin to high
    // read out photo
    analogRead(A0);
    // set moisture pin to high
    // read out moisture

    // water level
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);



    Serial.println("Sensor setup done");
}


void getSensorReading(char* formattedResponse, size_t maxResponseLen){
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // do this prettier at some point
    if(!isnan(temperature) || !isnan(humidity)){
        jsonResponse["sensor"] = "base";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = temperature;
        jsonResponse["humidity"] = humidity;
        jsonResponse["brightness"] = analogRead(A0);
        // switch with multiplexer
        jsonResponse["waterLevel"] = analogRead(A0);

    } else {
        // i should probably print or log this
        jsonResponse["sensor"] = "invalid";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = 0;
        jsonResponse["humidity"] = 0;
        jsonResponse["brightness"] = 0;
        jsonResponse["waterLevel"] = 0;
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
    jsonResponse["waterLevel"] = sensor_readings[temp].waterLevel;

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
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // do this prettier at some point
    if(!isnan(temperature) || !isnan(humidity)){
        return false;
    }

    sensor_readings[readingsListIndex].time = getEpochTime();
    Serial.println(sensor_readings[readingsListIndex].time);
    Serial.println("");
    sensor_readings[readingsListIndex].temperature = temperature;
    sensor_readings[readingsListIndex].humidity = humidity;
    sensor_readings[readingsListIndex].brightness = analogRead(A0);
    // switch with multiplexer
    sensor_readings[readingsListIndex].waterLevel = analogRead(A0);

    readingsListIndex = (readingsListIndex + 1) % NUM_READINGS;
    return true;
}

void printCurrentReading(){
    // brightness
    Serial.println("New Reading");
    Serial.print("brightness ");
    Serial.println(analogRead(A0));


    // and some more

    Serial.println("\n");
}
