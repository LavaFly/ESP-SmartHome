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


    //last time i wrote this, i set up all the modules inside their own little network, but i also
    //wrote the option once to use the base in ap mode and have the modules collected this way,
    //this run into issues at rougly 8 modules as clients, i tried to fix it for a short while, im pretty
    //sure i found something claiming that it has been fixed or there is a solution, but this was long ago
    //and i just bought a small router and set it up without internet connection. this way the base connects
    //to the local network with internet, requests the time from a time server, sets the time in a rtc module,
    //connects to the network with all the modules and functions as the time server for them.
    //but i will most likely connect all the modules to the internet again, as i have moved.
    //dont really know what to do with the code i have written to solve these issues, will maybe do some sort of
    //build flag, that way i can switch between these three setups, but this is a very low priority, as i wont
    //be switching often and therefore dont really need this function, for now just dirty hacking and commenting out
    //old code :D
    //stop me if you want or fix it yourself

    //softwareInit = softwareInit && buildAP();
    //softwareInit = softwareInit && buildNTPServer();

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
