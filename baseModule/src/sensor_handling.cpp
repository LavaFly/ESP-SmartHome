#include "sensor_handling.h"
#include "time_handling.h"

JsonDocument jsonResponse;
SensirionI2cScd30 co2Sensor;
RTC_DS3231 rtc;
static char errMsg[64]; // this is choosen w/o thought
static int16_t errNum;

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

void setTimeFromNTP();
void initSensor(){
    Serial.println("start init");

    Wire.begin();
    co2Sensor.begin(Wire, SCD30_I2C_ADDR_61);

    co2Sensor.stopPeriodicMeasurement();
    co2Sensor.softReset();
    co2Sensor.setMeasurementInterval(20);

    errNum = co2Sensor.startPeriodicMeasurement(0);
    if(errNum != NO_ERROR){
        Serial.println("Error trying to execute startPeriodicMeasurement(): ");
        Serial.println(errMsg);
        return;
    }

    rtc.begin();
    //use for testing
    //rtc.adjust(DateTime(__DATE__, __TIME__));

    setTimeFromNTP();

    // init photo
    analogRead(A0);

    Serial.println("Sensor setup done");
}

void getSensorReading(char* formattedResponse, size_t maxResponseLen){
    float temperature = 0.0;
    float humidity = 0.0;
    float co2 = 0.0;
    uint16_t dataReady = 0;

    dataReady = co2Sensor.getDataReady(dataReady);

    // the brightness measurement "cannot" fail due to software
    // reasons so only check the scd30 sensor
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

    errNum = co2Sensor.blockingReadMeasurementData(co2, temperature, humidity);

    if(errNum != NO_ERROR){
        return false;
    }

    sensor_readings[readingsListIndex].time = getEpochTime();
    sensor_readings[readingsListIndex].temperature = temperature;
    sensor_readings[readingsListIndex].humidity = humidity;
    sensor_readings[readingsListIndex].co2 = co2;
    sensor_readings[readingsListIndex].brightness = analogRead(A0);

    readingsListIndex = (readingsListIndex + 1) % NUM_READINGS;
    return true;
}

void printCurrentReading(){
    // brightness
    Serial.print("brightness ");
    Serial.println(analogRead(A0));
    Serial.println("\n");
}


unsigned long getSensorTime(){
    return rtc.now().unixtime();
}

void setSensorTime(unsigned long timeStamp){
    rtc.adjust(timeStamp);
}

void setTimeFromNTP() {

    if(updateTimeClient()){
        unsigned long epochTime = getEpochTime();

        rtc.adjust(DateTime(epochTime));
        Serial.println(epochTime);
    } else {
        Serial.println("fuck2");
    }

}
