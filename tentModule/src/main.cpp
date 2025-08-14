#include <Arduino.h>
#include "sd_handling.h"
#include "sensor_handling.h"
#include "webserver_handling.h"
#include "time_handling.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


void sensorCallback();
void printSensorsToDisplay();
void clearDisplay();


Adafruit_SSD1306 display(128, 64, &Wire, -1);

uint8_t previousButtonState = false;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting setup");
    pinMode(14, INPUT_PULLUP);
    pinMode(3, FUNCTION_3); // set rx to gpio, currently running out of pins...

    // i will outsource all parts relating to the display at some point, but havent decided if it should go
    // into its own led_handling like the ledModule, but this would kind of prohibit oled or lcd displays,
    // which could easily used for the same purpose, but i could alsp just treat it as an actor and put it
    // into actor_handling, large refactor with documentation coming soon, for sure...
    // ... at some point even proper testing ...INPUT_PULLUPINPUT_PULLUP
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { // Address 0x3D for 128x64
        Serial.println("mist");
        while(true){;}
    }
    delay(2000);

    Serial.println("starting");
    uint8_t hardwareInit = true;
    hardwareInit = hardwareInit && initSensor();
    //hardwareInit = hardwareInit && initSDCard();
    initSDCard();

    if(hardwareInit){
        Serial.println("Hardware init successful");
    } else {
        Serial.println("Hardware init failed");
    }

    uint8_t softwareInit = true;
    softwareInit = softwareInit && buildRouterConnection();
    softwareInit = softwareInit && buildTimeConnection();
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
    MDNS.update();
    loopOTA();

    setTimerMinutesCallback(10, &sensorCallback);

    // on button press
    uint8_t currentButtonState = digitalRead(3);
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


void printSensorsToDisplay(){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 30);

    float temp = getLatestTemperature();
    float humidity = getLatestHumidity();
    float resistance = getLatestResistance();
    uint16_t brightness = getLatestBrightness();
    uint8_t co2 = getLatestCO2();
    display.printf(" T=%2.1fC      H=%3.1f%\n", temp, humidity);
    display.printf("       B=%u\n", brightness);
    display.printf(" Co2=%u     Ohm=%3.1f%\n", co2, resistance);
    display.display();
}

void clearDisplay(){
    display.clearDisplay();
    display.display();
}
