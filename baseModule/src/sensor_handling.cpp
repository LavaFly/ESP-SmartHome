#include "sensor_handling.h"
#include "time_handling.h"

JsonDocument jsonResponse;
SensirionI2cScd30 co2Sensor;
RTC_DS3231 rtc;
static char errMsg[64]; // this is choosen w/o thought :D
static int16_t errNum;


/**
 * @brief This structure holds the information of a sensor reading
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

// this array is used as a ring buffer and holds the sensor information
// readingsListIndex is the index of the newest reading
sensor_reading sensor_readings[NUM_READINGS];
uint8_t readingsListIndex = 0;
uint8_t latestReading = 0; // this is a bad workaround, as i havent decided on how to properly fix the wrong implementation of the listIndex

uint8_t initSensor(){
    // should check the following statements for success
    Wire.begin();
    co2Sensor.begin(Wire, SCD30_I2C_ADDR_61);

    co2Sensor.stopPeriodicMeasurement();
    co2Sensor.softReset();
    // this makes it so that the blockingReadMeasurementData() method
    // and in turn the updateSensorValues function takes up to 20s !!!
    //co2Sensor.setMeasurementInterval(10);

    errNum = co2Sensor.startPeriodicMeasurement(0);
    if(errNum != NO_ERROR){
        Serial.println("Error trying to execute startPeriodicMeasurement(): ");
        Serial.println(errMsg);
        return 0;
    }

    rtc.begin();
    //use for testing
    //rtc.adjust(DateTime(__DATE__, __TIME__));
    setTimeFromNTP();

    return 1;
}

uint8_t getSensorReading(char* formattedResponse, size_t maxResponseLen){
    float temperature;
    float humidity;
    float co2;
    uint16_t dataReady = 0;

    //co2Sensor.awaitDataReady();
    dataReady = co2Sensor.getDataReady(dataReady);


    //co2Sensor.readMeasurementData(co2, temperature, humidity);
    uint8_t readingSuccessful = false;


    // the brightness measurement "cannot" fail due to software
    // reasons so only check the scd30 sensor

    uint32_t epochTime = getEpochTime();

    if(dataReady == 0){
        Serial.println("no data ready, reading failed");
        // i should probably print or log this
        jsonResponse["sensor"] = "invalid";
        jsonResponse["time"] = epochTime;
        jsonResponse["temperature"] = 23;
        jsonResponse["humidity"] = 30;
        jsonResponse["co2"] = 890;
        jsonResponse["brightness"] = 0;
    } else {
        //co2Sensor.readMeasurementData(co2, temperature, humidity);
        Serial.println("got data");
        Serial.print("Temp = ");
        Serial.println(temperature);

        jsonResponse["sensor"] = "base";
        jsonResponse["time"] = epochTime;
        jsonResponse["temperature"] = temperature;
        jsonResponse["humidity"] = humidity;
        jsonResponse["co2"] = co2;
        jsonResponse["brightness"] = analogRead(A0);
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

    jsonResponse["sensor"] = "base";
    jsonResponse["time"] = sensor_readings[temp].time;
    jsonResponse["temperature"] = sensor_readings[temp].temperature;
    jsonResponse["humidity"] = sensor_readings[temp].humidity;
    jsonResponse["co2"] = sensor_readings[temp].co2;
    jsonResponse["brightness"] = sensor_readings[temp].brightness;

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
    // if reading values from the sensor fails, return false, else store values and return true
    float temperature = 0.0;
    float humidity = 0.0;
    float co2 = 0.0;


    errNum = co2Sensor.blockingReadMeasurementData(co2, temperature, humidity);
    Serial.print("new reading, temp = ");
    Serial.println(temperature);
    // as long as the periodic interval(see initSensor) is larger than the interval
    // at which this method is called(aka setTimerSeconds/MinutesCallback(interval)
    // then this should be fine
    // i think
    //
    // it didnt work as planned, but will revisit this at some point
    /**
    uint16_t readyFlag = 0;
    errNum = co2Sensor.getDataReady(readyFlag);
    if(readyFlag != NO_ERROR){
        Serial.println("Sensor data not ready");
        return 0;
    }
    errNum = co2Sensor.readMeasurementData(co2, temperature, humidity);
    **/

    if(errNum != NO_ERROR){
        return 0;
    }

    sensor_readings[readingsListIndex].time = getEpochTime();
    sensor_readings[readingsListIndex].temperature = temperature;
    sensor_readings[readingsListIndex].humidity = humidity;
    sensor_readings[readingsListIndex].co2 = co2;
    sensor_readings[readingsListIndex].brightness = analogRead(A0);

    latestReading = readingsListIndex;
    readingsListIndex = (readingsListIndex + 1) % NUM_READINGS;
    return 1;
}


uint32_t getSensorTime(){
    return rtc.now().unixtime();
}


uint8_t setSensorTime(unsigned long timeStamp){
    //if(rtc.isrunning()){
    //    rtc.adjust(timeStamp);
    //    Serial.println("test this method");
    //    return 1;
    //}

    // i havent tested the isrunning() function
    // but cant right now, so just in case
    rtc.adjust(timeStamp);
    return 1;
}


uint8_t setTimeFromNTP() {
    if(updateTimeClient()){
        unsigned long epochTime = getEpochTime();
        rtc.adjust(DateTime(epochTime));
        return 1;
    }
    return 0;
}


void printCurrentReading(){
    Serial.println("Not implemented");
}


float getLatestTemperature(){
    return sensor_readings[latestReading].temperature;
}

float getLatestHumidity(){
    return sensor_readings[latestReading].humidity;
}

uint16_t getLatestBrightness(){
    return sensor_readings[latestReading].brightness;
}

float getLatestCO2(){
    return sensor_readings[latestReading].co2;
}
