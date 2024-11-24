#include "sensor_handling.h"
#include "time_handling.h"

JsonDocument jsonResponse;
DHT dht(STATUS_PIN, DHTTYPE);

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
    float waterLevel;
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
    setAnalogToBrightness();
    analogRead(A0);

    // set moisture pin to high
    // read out moisture
    setAnalogToMoisture();
    analogRead(A0);

    // water level
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Serial.println("Sensor setup done");
}


void getSensorReading(char* formattedResponse, size_t maxResponseLen){
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();


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


    // do this prettier at some point
    if(!isnan(temperature) || !isnan(humidity)){
        jsonResponse["sensor"] = "base";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = temperature;
        jsonResponse["humidity"] = humidity;
        setAnalogToBrightness();
        jsonResponse["brightness"] = analogRead(A0);
        // switch with multiplexer
        setAnalogToMoisture();
        jsonResponse["waterLevel"] = distance;

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
    if(isnan(temperature) || isnan(humidity)){
        return false;
    }
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
    Serial.println(sensor_readings[readingsListIndex].time);
    Serial.println("");
    sensor_readings[readingsListIndex].temperature = temperature;
    sensor_readings[readingsListIndex].humidity = humidity;
    setAnalogToBrightness();
    sensor_readings[readingsListIndex].brightness = analogRead(A0);
    // switch with multiplexer
    setAnalogToMoisture();
    sensor_readings[readingsListIndex].waterLevel = distance;

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

void setAnalogToBrightness(){
    // set correct Pin High
}

void setAnalogToMoisture(){
    // set correct Pin Low
}
