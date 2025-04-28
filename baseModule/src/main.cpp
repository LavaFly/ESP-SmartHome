#include <Arduino.h>
#include "sensor_handling.h"
#include "webserver_handling.h"
#include "time_handling.h"

String serialInput;

void sensorCallback();

void setup() {
    Serial.begin(9600);
    Serial.println("Starting setup");
    uint8_t hardwareInit = true;

    // the order of the following build/init functions is important!
    uint8_t softwareInit = true;
    softwareInit = softwareInit && buildRouterConnection();
    softwareInit = softwareInit && buildTimeConnection();

    hardwareInit = hardwareInit && initSensor();

    if(hardwareInit){
        Serial.println("Hardware init successful");
    } else {
        Serial.println("Hardware init failed");
    }

    buildAP();
    buildNTPServer();

    initWebserver();
    setupMDNS();

    if(softwareInit){
        Serial.println("Software init successful");
    } else {
        Serial.println("Software init failed");
    }

    Serial.println("Ending setup\n");
}

void loop() {
    yield();
    MDNS.update();
    loopOTA();

    setTimerMinutesCallback(10, &sensorCallback);
}

void sensorCallback(){
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
