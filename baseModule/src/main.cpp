#include <Arduino.h>
#include "led_handling.h"

// show time on led-wall when button is pressed

#define BUTTON_PIN 5

int currentButtonState;
int lastButtonState;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting...");

    pinMode(BUTTON_PIN, INPUT);

    // initialise connection to the led-wall
    buildLedConnection();
    initialiseLedMap();

    currentButtonState = digitalRead(BUTTON_PIN);
}

void loop() {
    lastButtonState = currentButtonState;
    currentButtonState = digitalRead(BUTTON_PIN);

    if(lastButtonState == HIGH && currentButtonState == LOW){
        // detected button press
        // show time
        Serial.println("showing time");
        uint8_t hour = 9;
        uint8_t minute = 45;
        projectTime(hour, minute);
        delay(5000);
        clearActiveLeds();

        Serial.println("showing time again");
        projectTime(hour + 6, minute + 4);
        delay(5000);
        allLEDSoff();
    }
}
