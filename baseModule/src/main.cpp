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

    currentButtonState = digitalRead(BUTTON_PIN);
}

void loop() {
    lastButtonState = currentButtonState;
    currentButtonState = digitalRead(BUTTON_PIN);

    if(lastButtonState == HIGH && currentButtonState == LOW){
        // detected button press
        // show time
    }
}
