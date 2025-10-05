#include "touch_handling.h"


const uint32_t DEBOUNCE = 10;
const uint32_t SINGLE_PRESS = 200;
const uint32_t EVENT_TIMEOUT = 700;
const uint32_t DOUBLE_PRESS_TIME = 200;
const uint32_t HOLDING_TIME = 800;

enum ButtonState{
    IDLE,
    HOLDING,
    FIRST_PRESS_DETECTED,
    SINGLE_PRESS_MODE,
    DOUBLE_PRESS_MODE
};

typedef struct {
    ButtonState currentState = IDLE;
    uint32_t timeoutStart;
    uint32_t lastPressTime;
    uint32_t initialPress;
    uint8_t followingButtonPresses;
    uint8_t buttonPressed;
    uint8_t previousButtonState;
    uint8_t buttonPin;
    // idea of having a pointer to the handleSinglePress and handleDoublePress Methods
} touchButton;

touchButton buttonList[NUM_BUTTONS];
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

void resetStateMachine(uint8_t indexInButtonList);
void handleSinglePress(uint8_t numberOfFollowingPresses);
void handleDoublePress(uint8_t numberOfFollowingPresses);
void handleHolding();

// this is just lazy
extern uint8_t httpGetRequestIgnoreResponse(const char* path);
extern void getWeatherDescription(char* weatherDescription);
extern void webSocketSend(char* inputText);

void touchLoop(){
    for(uint8_t i = 0; i < NUM_BUTTONS; i++){
        handleButtonInput(i);
        updateStateMachine(i);
    }
}

void handleButtonInput(uint8_t indexInButtonList){
    uint8_t currentButtonState = digitalRead(buttonList[indexInButtonList].buttonPin);
    uint32_t currentTime = millis();

    //i know i shouldnt manipulate currentState inside the buttonInput method and instead handle
    //this inside the updateStateMachine method, as inside here it is a side effect
    //
    //but i am feely awfully lazy right now
    if(buttonList[indexInButtonList].previousButtonState == LOW && currentButtonState == HIGH){
        buttonList[indexInButtonList].initialPress = currentTime;
    }

    if(buttonList[indexInButtonList].previousButtonState == HIGH && currentButtonState == HIGH && (currentTime - buttonList[indexInButtonList].initialPress) > HOLDING_TIME){
        buttonList[indexInButtonList].currentState = HOLDING;
    }
    // check if holding
    if(buttonList[indexInButtonList].previousButtonState == HIGH && currentButtonState == LOW){
        delay(DEBOUNCE); // check for debounce
        if(digitalRead(buttonList[indexInButtonList].buttonPin) == LOW){
            if(buttonList[indexInButtonList].currentState == HOLDING){
                // in my case, i dont really care for the release of a held button
                resetStateMachine(indexInButtonList);
            } else {
                buttonList[indexInButtonList].buttonPressed = true;
                buttonList[indexInButtonList].lastPressTime = currentTime;
            }
        }
    }
    buttonList[indexInButtonList].previousButtonState = currentButtonState;
}

void updateStateMachine(uint8_t indexInButtonList){
    uint32_t currentTime = millis();

    switch(buttonList[indexInButtonList].currentState){
        case IDLE:
            if(buttonList[indexInButtonList].buttonPressed){
                buttonList[indexInButtonList].currentState = FIRST_PRESS_DETECTED;
                buttonList[indexInButtonList].timeoutStart = currentTime;
                buttonList[indexInButtonList].followingButtonPresses = 0;
                buttonList[indexInButtonList].buttonPressed = false;
            }
            break;

        case FIRST_PRESS_DETECTED:
            if(buttonList[indexInButtonList].buttonPressed && (currentTime - buttonList[indexInButtonList].lastPressTime) <= DOUBLE_PRESS_TIME){
                buttonList[indexInButtonList].currentState = DOUBLE_PRESS_MODE;
                buttonList[indexInButtonList].timeoutStart = currentTime;
                buttonList[indexInButtonList].buttonPressed = false;
            } else if((currentTime - buttonList[indexInButtonList].lastPressTime) > DOUBLE_PRESS_TIME){
                buttonList[indexInButtonList].currentState = SINGLE_PRESS_MODE;
                buttonList[indexInButtonList].timeoutStart = currentTime;
            }
            break;

        case DOUBLE_PRESS_MODE:
            if(buttonList[indexInButtonList].buttonPressed){
                if(buttonList[indexInButtonList].followingButtonPresses < 4){
                    buttonList[indexInButtonList].followingButtonPresses++;
                }
                buttonList[indexInButtonList].timeoutStart = currentTime;
                buttonList[indexInButtonList].buttonPressed = false;
            } else if((currentTime - buttonList[indexInButtonList].timeoutStart) > EVENT_TIMEOUT){
                handleDoublePress(buttonList[indexInButtonList].followingButtonPresses);
                resetStateMachine(indexInButtonList);
            }
            break;

        case SINGLE_PRESS_MODE:
            if(buttonList[indexInButtonList].buttonPressed){
                if(buttonList[indexInButtonList].followingButtonPresses < 4){
                    buttonList[indexInButtonList].followingButtonPresses++;
                }
                buttonList[indexInButtonList].timeoutStart = currentTime;
                buttonList[indexInButtonList].buttonPressed = false;
            } else if((currentTime - buttonList[indexInButtonList].timeoutStart) > EVENT_TIMEOUT){
                //yeah...
                if(indexInButtonList == 0){
                    handleSinglePress(buttonList[indexInButtonList].followingButtonPresses);
                } else {
                    handleSecondButton(buttonList[indexInButtonList].followingButtonPresses);
                }
                resetStateMachine(indexInButtonList);
            }
            break;

        case HOLDING:
            //not even gonna hide this, magic numbers all the way
            if(indexInButtonList == 1){
                handleHolding();
            }
            break;
    }
}

void resetStateMachine(uint8_t indexInButtonList){
    buttonList[indexInButtonList].currentState = IDLE;
    buttonList[indexInButtonList].followingButtonPresses = 0;
    buttonList[indexInButtonList].buttonPressed = false;
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
    //make api request and parse
    char* weatherDescription = (char*)malloc(25* sizeof(char));//hehe, magic
    uint8_t forecastTimes[4] = {7, 12, 17, 22}
    getWeatherDescription(weatherDescription, forecastTimes[numberOfFollowingPresses]);

    //websocket to led
    webSocketSend(weatherDescription);
}


uint8_t setupButtons(){
    // create both buttons
    touchButton lightCtrl;
    lightCtrl.buttonPin = BUTTON_PIN1;
    buttonList[0] = lightCtrl;

    touchButton ledCtrl;
    ledCtrl.buttonPin = BUTTON_PIN2;
    buttonList[1] = ledCtrl;

    return 1;
}
