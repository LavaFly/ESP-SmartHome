#include "sensor_handling.h"
#include "time_handling.h"

JsonDocument jsonResponse;
SensirionI2cScd30 co2Sensor;
static char errMsg[64]; // this is choosen w/o thought
static int16_t errNum;

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
    // scd30
    float temperature;
    float humidity;
    float co2;
    // photo
    uint16_t brightness;
} sensor_reading;
sensor_reading sensor_readings[NUM_READINGS];

void initSensor(){
    Serial.println("start init");

    Wire.begin();
    co2Sensor.begin(Wire, SCD30_I2C_ADDR_61);

    co2Sensor.stopPeriodicMeasurement();
    co2Sensor.softReset();
    co2Sensor.setMeasurementInterval(10);

    errNum = co2Sensor.startPeriodicMeasurement(0);
    if(errNum != NO_ERROR){
        Serial.println("Error trying to execute startPeriodicMeasurement(): ");
        Serial.println(errMsg);
        return;
    }


    // init co2
    /**
    will move this somewhere else, but keep for now
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
    float temperature = 0.0;
    float humidity = 0.0;
    float co2 = 0.0;
    uint16_t dataReady = 0;

    dataReady = co2Sensor.getDataReady(dataReady);

    if(dataReady == 0){
        Serial.println("no data ready, reading failed");

        // i should probably print or log this
        jsonResponse["sensor"] = "invalid";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = 0;
        jsonResponse["humidity"] = 0;
        jsonResponse["co2"] = 0;
        jsonResponse["brightness"] = 0;
    } else {
        co2Sensor.readMeasurementData(co2, temperature, humidity);

        jsonResponse["sensor"] = "base";
        jsonResponse["time"] = getEpochTime();
        jsonResponse["temperature"] = temperature;
        jsonResponse["humidity"] = humidity;
        jsonResponse["co2"] = co2;
        jsonResponse["brightness"] = analogRead(A0);
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
    //jsonResponse["quality"] = sensor_readings[temp].quality;
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
    float temperature = 0.0;
    float humidity = 0.0;
    float co2 = 0.0;
    uint16_t dataReady = 0;

    dataReady = co2Sensor.getDataReady(dataReady);
    if(dataReady != 1){
        return false;
    }
    co2Sensor.readMeasurementData(co2, temperature, humidity);

    sensor_readings[readingsListIndex].time = getEpochTime();
    Serial.println(sensor_readings[readingsListIndex].time);
    Serial.println("");
    sensor_readings[readingsListIndex].temperature = temperature;
    sensor_readings[readingsListIndex].humidity = humidity;
    sensor_readings[readingsListIndex].co2 = co2;
    sensor_readings[readingsListIndex].brightness = analogRead(A0);

    readingsListIndex = (readingsListIndex + 1) % NUM_READINGS;
    return true;
}

void printCurrentReading(){
    // brightness
    Serial.println("New Reading");
    Serial.print("brightness ");
    Serial.println(analogRead(A0));

    Serial.println("\n");
}
