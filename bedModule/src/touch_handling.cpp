#include "touch_handling.h"

#define BUTTON_PIN 13

const uint32_t DEBOUNCE = 10;
const uint32_t SINGLE_PRESS = 200;
const uint32_t EVENT_TIMEOUT = 700;
const uint32_t DOUBLE_PRESS_TIME = 200;

//uint8_t currentState;

uint8_t previousButtonState;

uint32_t timeoutStart;
uint32_t lastPressTime;
uint8_t followingButtonPresses;
uint8_t buttonPressed;

enum ButtonState{
    IDLE,
    FIRST_PRESS_DETECTED,
    SINGLE_PRESS_MODE,
    DOUBLE_PRESS_MODE
};

ButtonState currentState = IDLE;

void handleSinglePress(uint8_t numberOfFollowingPresses);
void handleDoublePress(uint8_t numberOfFollowingPresses);
void resetStateMachine();

void touchLoop(){
    handleButtonInput();
    updateStateMachine();
}

void handleButtonInput(){
    uint8_t currentButtonState = digitalRead(BUTTON_PIN);
    if(previousButtonState == HIGH && currentButtonState == LOW){
        delay(DEBOUNCE); // check for debounce
        if(digitalRead(BUTTON_PIN) == LOW){
            buttonPressed = true;
            lastPressTime = millis();
        }
    }
    previousButtonState = currentButtonState;
}

void updateStateMachine(){
    uint32_t currentTime = millis();

    switch(currentState){
        case IDLE:
            if(buttonPressed){
                currentState = FIRST_PRESS_DETECTED;
                timeoutStart = currentTime;
                followingButtonPresses = 0;
                Serial.println("first press");
                buttonPressed = false;
            }
            break;

        case FIRST_PRESS_DETECTED:
            if(buttonPressed && (currentTime - lastPressTime) <= DOUBLE_PRESS_TIME){
                currentState = DOUBLE_PRESS_MODE;
                timeoutStart = currentTime;
                Serial.println("now double mode");
                buttonPressed = false;
            } else if((currentTime - lastPressTime) > DOUBLE_PRESS_TIME){
                currentState = SINGLE_PRESS_MODE;
                timeoutStart = currentTime;
                Serial.println("single press mode");
            }
            break;

        case DOUBLE_PRESS_MODE:
            if(buttonPressed){
                followingButtonPresses++;
                timeoutStart = currentTime;
                Serial.println("add press double");
                buttonPressed = false;
            } else if((currentTime - timeoutStart) > EVENT_TIMEOUT){
                handleDoublePress(followingButtonPresses);
                resetStateMachine();
            }
            break;

        case SINGLE_PRESS_MODE:
            if(buttonPressed){
                followingButtonPresses++;
                timeoutStart = currentTime;
                Serial.println("add press single");
                buttonPressed = false;
            } else if((currentTime - timeoutStart) > EVENT_TIMEOUT){
                handleSinglePress(followingButtonPresses);
                resetStateMachine();
            }
            break;
    }
}

void resetStateMachine(){
    currentState = IDLE;
    followingButtonPresses = 0;
    buttonPressed = false;
}

void handleSinglePress(uint8_t numberOfFollowingPresses){
    Serial.println("");
    Serial.print("i was pressed once and ");
    Serial.println(numberOfFollowingPresses);
    Serial.println("");
}
void handleDoublePress(uint8_t numberOfFollowingPresses){
    Serial.println("");
    Serial.print("i was pressed double and ");
    Serial.println(numberOfFollowingPresses);
    Serial.println("");
}
