#include "sensor_handling.h"

DHT dht(2, DHTTYPE);

JsonDocument jsonResponse;

void initSensor(){
    dht.begin();
}


// ideally return string containing
// the sensor readings as this will
// simplify further implementations
void getSensorReading(char* formattedResponse, size_t maxResponseLen){
    // fix later
    float humidity = 0; //dht.readHumidity();
    float temperature = 0; //dht.readTemperature();
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
