#include <Arduino.h>
#include "sensor_handling.h"
#include "webserver_handling.h"
#include "time_handling.h"
#include "sd_handling.h"

String serialInput;

void sensorCallback();

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");

    delay(1000);
    //connect to the internet
    buildRouterConnection();
    buildTimeConnection(); // this is obsolete
    //connect to the sensor
    //get and set the time
    initSensor();
    //disconnect from the network

    //connect to other network
    //setup server
    setupNTPServer();

    //setupNTPServer();
    //initWebserver();
    setupMDNS();
    Serial.println("setup done");
    //printWeatherData();
    //printForecastData();


    //initSDCard();
    //createDir();
}

void loop() {
    MDNS.update();
    loopOTA();

    setTimerSecondsCallback(10, &sensorCallback);
}

void sensorCallback(){
    Serial.println(getSensorTime());
    Serial.println(getEpochTime());
    if(!updateTimeClient()){
        Serial.println("failed to update timeClient");
    }
    if(!updateSensorValues()){
        Serial.println("failed to read out sensors");
    }

}
/**
// write own values to sd card
char* sensorData = (char*)malloc(sizeof(char) * 180); // rougly 124 will be used

getSensorReading(sensorData, 180);

writeJsonToFile(sensorData);

free(sensorData);

// write pcModule values to sdcard
const char* responseData = httpGetRequest("http://pcModule.local/sensorReading")->c_str();
writeJsonToFile(responseData);
**/
