#include "touch_handling.h"
// tap type     |       timing
// single       |       timePressed = 10..200ms
// double       |       pauseTime = 100..1000ms
// n-tap        |
// tap+hold     |       holdTime = 200..5000ms


// everything w/ single press -> ledModule
//               double press -> lightingModule
uint8_t statusOfControl = 0; // bad name
uint8_t previousButtonStatus = 0;
uint32_t buttonDownTime = 0;
uint32_t buttonLastReleasedTime = 0;


// current idea
// do main loop where checking if currently pressed
//  if
//   check which state i currenty am in, based on the value of currentState
//    0 -> reset
//    1 -> single tap
//    2 -> double tap
//  else
//   check if any timer is present and has run out, perform action based on currentState
//    0 -> impossible, but who knows
//    1 -> turn on ledModule lighting
//    2 -> turn on lightingModule lighting
//    3 -> turn on ledModule time
//    4 -> specify brightness for ledModule
//    5 -> specify brightness for lighting
//    6 -> specify color for ledModule



// returns the state of the automata
uint8_t checkForEvent(){
    // if is pressed and
    // put action into queue
    uint8_t currentButtonStatus = digitalRead(13);
    uint32_t currentTime = millis();
    char buff[100];
    uint8_t retCode = 0;
    //Serial.print("entering method at: ");
    //Serial.println(currentTime);

    // if button is currently pressed and was not pressed before, save timestamp
    if(currentButtonStatus == HIGH && previousButtonStatus == LOW){
        buttonDownTime = currentTime;
        Serial.println("curr = high, prev = low, setting buttonDownTime");
        sprintf(buff, "c = %u, b = %u, pB = %u\n", statusOfControl, currentButtonStatus, previousButtonStatus);
        //Serial.print(buff);
        sprintf(buff, "bd = %l, blr = %l, curr = %l\n\n", buttonDownTime, buttonLastReleasedTime, currentTime);
        //Serial.print(buff);

    // if button is currently pressed and was pressed before, check timestamp
    } else if(currentButtonStatus == HIGH && previousButtonStatus == HIGH){
        Serial.println("curr = high, prev = high, default");
        sprintf(buff, "c = %u, b = %u, pB = %u\n", statusOfControl, currentButtonStatus, previousButtonStatus);
        //Serial.print(buff);
        sprintf(buff, "bd = %l, blr = %l, curr = %l\n\n", buttonDownTime, buttonLastReleasedTime, currentTime);
        //Serial.print(buff);

        // if time over
        if((currentTime - buttonDownTime) > 200){
            //Serial.println("curr = high, prev = high, holding the button");
            sprintf(buff, "c = %u, b = %u, pB = %u\n", statusOfControl, currentButtonStatus, previousButtonStatus);
            //Serial.print(buff);
            sprintf(buff, "bd = %l, blr = %l, curr = %l\n\n", buttonDownTime, buttonLastReleasedTime, currentTime);
            //Serial.print(buff);

            // now holding the button down
            retCode = 3;
        }

    // if button is not currently pressed and was pressed before, check timestamp
    } else if(currentButtonStatus == LOW && previousButtonStatus == HIGH){
        buttonLastReleasedTime = currentTime;

        Serial.println("curr = low, prev = high, default");
        sprintf(buff, "c = %u, b = %u, pB = %u\n", statusOfControl, currentButtonStatus, previousButtonStatus);
        //Serial.print(buff);
        sprintf(buff, "bd = %l, blr = %l, curr = %l\n\n", buttonDownTime, buttonLastReleasedTime, currentTime);
        //Serial.print(buff);


        // if
        //  time was to short for anything, jitter
        if((buttonLastReleasedTime - buttonDownTime) < 10){

            Serial.println("curr = low, prev = high, jittwe");
            sprintf(buff, "c = %u, b = %u, pB = %u\n", statusOfControl, currentButtonStatus, previousButtonStatus);
            //Serial.print(buff);
            sprintf(buff, "bd = %l, blr = %l, curr = %l\n\n", buttonDownTime, buttonLastReleasedTime, currentTime);
            //Serial.print(buff);
            retCode = 0;


        //  time was long enough for button press
        } else if((buttonLastReleasedTime - buttonDownTime) < 200){
            Serial.println("curr = low, prev = high, normal press");
            sprintf(buff, "c = %u, b = %u, pB = %u\n", statusOfControl, currentButtonStatus, previousButtonStatus);
            //Serial.print(buff);
            sprintf(buff, "bd = %l, blr = %l, curr = %l\n\n", buttonDownTime, buttonLastReleasedTime, currentTime);
            //Serial.print(buff);
            statusOfControl = 1;
            retCode = 1;

        //  time was to long for press, was holding(do not to action for normal release)
        } else {
            Serial.println("curr = low, prev = high, was holding");
            sprintf(buff, "c = %u, b = %u, pB = %u\n", statusOfControl, currentButtonStatus, previousButtonStatus);
            //Serial.print(buff);
            sprintf(buff, "bd = %l, blr = %l, curr = %l\n\n", buttonDownTime, buttonLastReleasedTime, currentTime);
            //Serial.print(buff);
            retCode = 0;
        }

    } else { // current low, previous low
        // reset all after 1 second of nothing
        if((currentTime - buttonLastReleasedTime) > 1000){
            buttonDownTime = 0;
            buttonLastReleasedTime = 0;
            //Serial.println("curr = low, prev = low, resetting all");
            sprintf(buff, "c = %u, b = %u, pB = %u\n", statusOfControl, currentButtonStatus, previousButtonStatus);
            //Serial.print(buff);
            sprintf(buff, "bd = %l, blr = %l, curr = %l\n\n", buttonDownTime, buttonLastReleasedTime, currentTime);
            //Serial.print(buff);
        }
    }

    previousButtonStatus = currentButtonStatus;
    return retCode;
}

void handleDoublePress(){
    //
}

void handleNPress(){
    // from to 1 to 4, different levels of brightness for selected light source
}

void handleHold(){

}

void handlePressThenHold(){

}
