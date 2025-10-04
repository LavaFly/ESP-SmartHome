#include "touch_handling.h"


const uint32_t DEBOUNCE = 10;
const uint32_t SINGLE_PRESS = 200;
const uint32_t EVENT_TIMEOUT = 700;
const uint32_t DOUBLE_PRESS_TIME = 200;
const uint32_t HOLDING_TIME = 800;

//uint8_t currentState;

uint8_t previousButtonState;

uint32_t timeoutStart;
uint32_t lastPressTime;
uint32_t initialPress;
uint8_t followingButtonPresses;
uint8_t buttonPressed;


enum ButtonState{
    IDLE,
    HOLDING,
    FIRST_PRESS_DETECTED,
    SINGLE_PRESS_MODE,
    DOUBLE_PRESS_MODE
};

ButtonState currentState = IDLE;

// new objective
//  have two buttons
//   one does all this current stuff and controlls the two lights
//    done
//   the other one displays the time, or the weather forecast
//    hold for i second for time
//    1 tap is forecast for 7:00
//    2 tap for 12:00
//    3 taps for 17:00
//    4 taps for 22:00
//
// what do i need to implement for this?
//  http client to request weather data from api
//  parse json response for relevant data
//  send data per websocket to led wall
//
//  setup second button
//   hw setup
//   create struct for button with relevant variables
//   flag for holding or double press(not the best solution but good enough)
//   modify methods
//   function pointer for handleMethods

void handleSinglePress(uint8_t numberOfFollowingPresses);
void handleDoublePress(uint8_t numberOfFollowingPresses);
void resetStateMachine();

// this is just lazy
extern uint8_t httpGetRequestIgnoreResponse(const char* path);
extern void getWeatherDescription(char* weatherDescription);

void touchLoop(){
    handleButtonInput();
    updateStateMachine();
}

void handleButtonInput(){
    uint8_t currentButtonState = digitalRead(BUTTON_PIN);
    uint32_t currentTime = millis();

    //i know i shouldnt manipulate currentState inside the buttonInput method and instead handle
    //this inside the updateStateMachine method, as inside here it is a side effect
    //
    //but i am feely awfully lazy right now
    if(previousButtonState == LOW && currentButtonState == HIGH){
        initialPress = currentTime;
    }

    if(previousButtonState == HIGH && currentButtonState == HIGH && (currentTime - initialPress) > HOLDING_TIME){
        currentState = HOLDING;
    }
    // check if holding
    if(previousButtonState == HIGH && currentButtonState == LOW){
        delay(DEBOUNCE); // check for debounce
        if(digitalRead(BUTTON_PIN) == LOW){
            if(currentState == HOLDING){
                // in my case, i dont really care for the release of a held button
                resetStateMachine();
            } else {
                buttonPressed = true;
                lastPressTime = currentTime;
            }
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
                buttonPressed = false;
            }
            break;

        case FIRST_PRESS_DETECTED:
            if(buttonPressed && (currentTime - lastPressTime) <= DOUBLE_PRESS_TIME){
                currentState = DOUBLE_PRESS_MODE;
                timeoutStart = currentTime;
                buttonPressed = false;
            } else if((currentTime - lastPressTime) > DOUBLE_PRESS_TIME){
                currentState = SINGLE_PRESS_MODE;
                timeoutStart = currentTime;
            }
            break;

        case DOUBLE_PRESS_MODE:
            if(buttonPressed){
                if(followingButtonPresses < 4){
                    followingButtonPresses++;
                }
                timeoutStart = currentTime;
                buttonPressed = false;
            } else if((currentTime - timeoutStart) > EVENT_TIMEOUT){
                handleDoublePress(followingButtonPresses);
                resetStateMachine();
            }
            break;

        case SINGLE_PRESS_MODE:
            if(buttonPressed){
                if(followingButtonPresses < 4){
                    followingButtonPresses++;
                }
                timeoutStart = currentTime;
                buttonPressed = false;
            } else if((currentTime - timeoutStart) > EVENT_TIMEOUT){
                handleSinglePress(followingButtonPresses);
                resetStateMachine();
            }
            break;

        case HOLDING:
            // this state is reached after the press timeout has been reached
            break;
    }
}

void resetStateMachine(){
    currentState = IDLE;
    followingButtonPresses = 0;
    buttonPressed = false;
}

void handleSinglePress(uint8_t numberOfFollowingPresses){
    char path[40] = "http://lighting.local/";
    if(numberOfFollowingPresses == 0){
        // append 'toggle'
        char aWord[7] = "toggle";
        strcat(path, aWord);
    } else {
        // append number but max 4
        uint8_t endOfString = strlen(path);
        path[endOfString] = '0' + numberOfFollowingPresses;
        path[endOfString + 1] = '\0';
    }
    Serial.println("sending req to");
    Serial.println(path);
    Serial.println("");
    httpGetRequestIgnoreResponse(path);
}
void handleDoublePress(uint8_t numberOfFollowingPresses){
    char path[40] = "http://led.local/";
    if(numberOfFollowingPresses == 0){
        // append 'toggle'
        char aWord[7] = "toggle";
        strcat(path, aWord);
    } else {
        // append number but max 4
        uint8_t endOfString = strlen(path);
        path[endOfString] = '0' + numberOfFollowingPresses;
        path[endOfString + 1] = '\0';
    }
    Serial.println("sending req to");
    Serial.println(path);
    Serial.println("");
    httpGetRequestIgnoreResponse(path);
}
void handleHolding(){
    Serial.println("handle holding");
    httpGetRequestIgnoreResponse("http://led.local/showTime");
}

//i dont like this name
void handleSecondButton(uint8_t numberOfFollowingPresses){
    //1 = 700, 2 = 12, 3 = 17, 4 = 22
    //make api request
    //parse response
    char* weatherDescription = (char*)malloc(25* sizeof(char));//hehe, magic
    getWeatherDescription(weatherDescription);
    //websocket to led
    //

}
