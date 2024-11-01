#include "sensor_handling.h"
#include "time_handling.h"

JsonDocument jsonResponse;
Adafruit_BME680 bme;

int pwmPin = 15;
int ppmrange = 5000;
unsigned long pwmtime;

int PPM = 0;
float pulsepercent=0;

uint8_t readingsListIndex = 0;

/**
 * @brief This Structure holds all information that is stored from a sensor reading
 *  ( the BME680 is capable of reading airpressure, though this is practically unchanging in my application and thus will not be stored )
 *
 */
typedef struct {
    uint32_t time;
    // bme680
    float temperature;
    float humidity;
    float quality;
    // co2 sensor
    float co2;
    // photo
    uint16_t brightness;
} sensor_reading;
sensor_reading sensor_readings[NUM_READINGS];

void initSensor(){
    Serial.println("start init");

    while(!bme.begin(119)){
        Serial.print(".");
        delay(50);
    }
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150);

    // is left out for now
    // init co2
    /**
    pinMode(pwmPin, INPUT);
    pwmtime = pulseIn(pwmPin, HIGH, 2000000) / 1000;
    float pulsepercent = pwmtime / 1004.0;
    PPM = ppmrange * pulsepercent;
    **/

    // init photo
    analogRead(A0);

    Serial.println("Sensor setup done");
}


void readCo2(){
    pwmtime = pulseIn(pwmPin, HIGH, 2000000) / 1000;
    float pulsepercent = pwmtime / 1004.0;
    PPM = ppmrange * pulsepercent;

    /* taken from
     * https://iotspace.dev/arduino-co2-sensor-mh-z19-beispiel-und-sketch/
     * <400  - frische Außenluft
     * <800  - hohe Raumluftqualität
     * <1000 - akzeptable "Pettenkoferzahl"
     * <2000 - hygienisch auffällig
     * >2000 - hygienisch inakzeptabel
     */
}

void getSensorReading(char* formattedResponse, size_t maxResponseLen){
    if(bme.performReading()){
        jsonResponse["sensor"] = "base";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = bme.temperature;
        jsonResponse["humidity"] = bme.humidity;
        jsonResponse["quality"] = bme.gas_resistance;
        jsonResponse["co2"] = 0;
        jsonResponse["brightness"] = analogRead(A0);

    } else {
        // i should probably print or log this
        jsonResponse["sensor"] = "invalid";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = 0;
        jsonResponse["humidity"] = 0;
        jsonResponse["quality"] = 0;
        jsonResponse["co2"] = 0;
        jsonResponse["brightness"] = 0;
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
    jsonResponse["quality"] = sensor_readings[temp].quality;
    jsonResponse["co2"] = sensor_readings[temp].co2;
    jsonResponse["brightness"] = sensor_readings[temp].brightness;

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
    // if reading values from the sensor fails, return false, else store values and return true
    if(!bme.performReading()){
        return false;
    }

    sensor_readings[readingsListIndex].time = getEpochTime();
    Serial.println(sensor_readings[readingsListIndex].time);
    Serial.println("");
    sensor_readings[readingsListIndex].temperature = bme.temperature;
    sensor_readings[readingsListIndex].humidity = bme.humidity;
    sensor_readings[readingsListIndex].quality = bme.gas_resistance;
    sensor_readings[readingsListIndex].co2 = readingsListIndex;
    sensor_readings[readingsListIndex].brightness = analogRead(A0);

    readingsListIndex = (readingsListIndex + 1) % NUM_READINGS;
    return true;
}

void printCurrentReading(){
    // brightness
    Serial.println("New Reading");
    Serial.print("brightness ");
    Serial.println(analogRead(A0));

    // BME680
    bme.performReading();
    Serial.print("temperature: ");
    Serial.println(bme.temperature);

    Serial.print("humidity: ");
    Serial.println(bme.humidity);
    Serial.print("gas_resistance: ");
    Serial.println(bme.gas_resistance);

    // CO2
    /**
    pwmtime = pulseIn(pwmPin, HIGH, 2000000) / 1000;
    float pulsepercent = pwmtime / 1004.0;
    PPM = ppmrange * pulsepercent;
    Serial.print("co2: ");
    Serial.println(PPM);
    **/

    // time
    //Serial.println("time: ");

    Serial.println("\n");
}
