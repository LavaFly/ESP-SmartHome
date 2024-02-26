#include "sensor_handling.h"

DHT dht(2, DHTTYPE);

JsonDocument jsonResponse;

float pollTemp = 1;
float pollHumid = 1;

void initSensor(){
    dht.begin();
}

void getSensorReading(char* formattedResponse, size_t maxResponseLen){
    float humidity = pollHumid;
    float temperature = pollTemp;
    Serial.print("temperature = ");
    Serial.println(temperature);
    Serial.print("humidity = ");
    Serial.println(humidity);
    if(isnan(humidity) || isnan(temperature)){
        jsonResponse["sensor"] = "invalid";
        jsonResponse["temperature"] = 0;
        jsonResponse["humidity"] = 0;
    } else {
        jsonResponse["sensor"] = "dht22";
        jsonResponse["temperature"] = temperature;
        jsonResponse["humidity"] = humidity;
    }
    jsonResponse.shrinkToFit();
    serializeJson(jsonResponse, formattedResponse, maxResponseLen);
}

void printReading(){
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    Serial.print("temperature ");
    Serial.println(temperature);
    Serial.print("humidity ");
    Serial.println(humidity);
    Serial.println();
}

void updateSensorValues(){
    /**
     * unfortunatly i cannot call the read-functions during the request
     * as this causes a massive error that i have not been able to fix yet
     * see https://github.com/esp8266/Arduino/issues/6811 for more info
     * the steps of increasing the stackSize in StackThunk.cpp or
     * the bufferSize in WiFiClientBearSSL.cpp have not worked
     **/
    pollTemp = dht.readTemperature();
    pollHumid = dht.readHumidity();
    Serial.println("updating values");
}
