#include "sensor_handling.h"

DHT dht(2, DHTTYPE);

void initSensor(){
    dht.begin();
}


// ideally return string containing
// the sensor readings as this will
// simplify further implementations
float getSensorReading(){
    //float humidity = dht.getHumidity();
    //float temperature = dht.getTemperature();
    return 0;
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
