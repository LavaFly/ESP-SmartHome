#include <Arduino.h>
#include "sensor_handling.h"
#include "webserver_handling.h"
#include "time_handling.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


String serialInput;

uint8_t previousButtonState;

void sensorCallback();
void printSensorsToDisplay();
void clearDisplay();

Adafruit_SSD1306 display(128, 64, &Wire, -1);
void setup() {
    Serial.begin(9600);
    Serial.println("Starting setup");
    uint8_t hardwareInit = true;
    pinMode(13, INPUT);


    // the order of the following build/init functions is important!
    uint8_t softwareInit = true;
    softwareInit = softwareInit && buildRouterConnection();
    softwareInit = softwareInit && buildTimeConnection();

    hardwareInit = hardwareInit && initSensor();

    // see note in tentHandling main
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
        Serial.println("mist");
        while(true){;}
    }

    if(hardwareInit){
        Serial.println("Hardware init successful");
    } else {
        Serial.println("Hardware init failed");
    }

    softwareInit = softwareInit && buildAP();
    softwareInit = softwareInit && buildNTPServer();

    softwareInit = softwareInit && initWebserver();
    softwareInit = softwareInit && setupMDNS();

    if(softwareInit){
        Serial.println("Software init successful");
    } else {
        Serial.println("Software init failed");
    }

    Serial.println("Ending setup\n");
}

void loop() {
    //yield();
    MDNS.update();
    loopOTA();

    //setTimerSecondsCallback(20, sensorCallback);
    setTimerMinutesCallback(10, &sensorCallback);

    uint8_t currentButtonState = digitalRead(13);
    if(currentButtonState == true && previousButtonState == false){
        clearDisplay();
        Serial.println("clearing");
    } else if(currentButtonState == false && previousButtonState == true){
        printSensorsToDisplay();
        Serial.println("printing");
    }
    previousButtonState = currentButtonState;
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
void printSensorsToDisplay(){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,30);
    float temp = getLatestTemperature();
    float humidity = getLatestHumidity();
    uint16_t brightness = getLatestBrightness();
    float co2 = getLatestCO2();
    display.printf(" T = %2.fC   H = %3.1f%\n", temp, humidity);
    display.printf("      B = %u\n", brightness);
    display.printf("   Co2 = %3.1f\n", co2);

    display.display();
}

void clearDisplay(){
    display.clearDisplay();
    display.display();
}
