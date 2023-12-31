#include "led_handling.h"
#include "characters.h"

// dimension of the led-wall
const uint8_t wallHeight = 7;
const uint8_t wallWidth = 23;
const uint8_t numberOfLeds = 149;

CRGB leds[numberOfLeds];

uint8_t activeLeds[numberOfLeds];

uint8_t* ledMap[7];

//extern uint8_t numbers[10][5][3];

// width = ((num - (height + 1)/2)^2) / height + (height + 1/2)


void initialiseLedMap(){
    // i will clean this mess up another time, it works for now
    // docu will also follow soon

    uint8_t segmentWidth = 0;
    for(uint8_t i = 0; i < wallHeight; i++){
        segmentWidth = wallWidth - abs(-(wallHeight / 2) + i);
        *(ledMap + i) = new uint8_t[segmentWidth];
    }

    for(uint8_t y = 0; y < wallHeight; y++){
        segmentWidth = wallWidth - abs(-(wallHeight / 2) + y);
        for(uint8_t x = 0; x < segmentWidth; x++){
            ledMap[y][x] = 0;
        }
    }

    initialiseLedMapBorders();

    uint8_t ledLeftNeighbour = 0;
    for(uint8_t y = 0; y < wallHeight; y++){
        segmentWidth = wallWidth - abs(-(wallHeight / 2) + y);
        for(uint8_t x = 0; x < segmentWidth; x++){
            if( ledMap[y][x] == 0 && x > 0){
                ledLeftNeighbour = 2 * wallHeight - (2 * y + 1);
                ledMap[y][x] = ledMap[y][x - 1] + ledLeftNeighbour;
                ledMap[y][x + 1] = ledMap[y][x] + (2 * y + 1);
            }
        }
    }
}

void initialiseLedMapBorders(){
    ledMap[0][0] = 6;
    ledMap[1][0] = 5;
    ledMap[1][1] = 7;
    ledMap[2][0] = 4;
    ledMap[2][1] = 8;
    ledMap[3][0] = 0;
    ledMap[3][1] = 3;
    ledMap[3][2] = 9;
    ledMap[4][0] = 2;
    ledMap[4][1] = 10;
    ledMap[5][0] = 1;
    ledMap[5][1] = 11;
    ledMap[6][0] = 12;

    ledMap[0][19] = 145;
    ledMap[1][19] = 133;
    ledMap[1][20] = 144;
    ledMap[2][20] = 143;
    ledMap[2][21] = 146;
    ledMap[3][20] = 135;
    ledMap[3][21] = 142;
    ledMap[3][22] = 147;
    ledMap[4][20] = 141;
    ledMap[4][21] = 148;
    ledMap[5][19] = 137;
    ledMap[5][20] = 140;
    ledMap[6][19] = 139;
}
void buildLedConnection(){
    pinMode(LEDPIN, OUTPUT);
    FastLED.addLeds<WS2812, LEDPIN, GRB>(leds, numberOfLeds);
}

void projectDigit(uint8_t digit, uint8_t xOffset, uint8_t yOffset){
    uint8_t yPosition, xPosition, ledIndex;
    for(uint8_t y = 0; y < 5; y++){
        for(uint8_t x = 0; x < 3; x++){
            if(numbers[digit][y][x]){
                yPosition = y + yOffset;
                xPosition = x + xOffset - ((yPosition > wallHeight / 2) ? (yPosition - wallHeight / 2) : 0);
                if(yPosition < wallHeight || xPosition < wallWidth - abs(-(wallHeight / 2) + yPosition)){
                    ledIndex = ledMap[yPosition][xPosition];
                    leds[ledIndex] = CRGB::White;
                }
            }
        }
    }
    FastLED.show();
}

uint8_t getNumberOfDigits(uint16_t number){
    if(number == 0) return 1;
    return floor(log10(abs(number))) + 1;
}

void projectNumber(uint16_t number){
    uint8_t numberOfDigits = getNumberOfDigits(number);

    uint8_t xOffset;
    uint8_t digit;
    for(uint8_t i = 0; i < numberOfDigits; i++){
        xOffset = wallWidth - (4 * i) - 6;
        digit = number / ((int)pow(10, i)) % 10;
        projectDigit(digit, xOffset, 1);
    }
    FastLED.show();
}

void projectNumber(uint16_t number, uint8_t xOffset, uint8_t yOffset){
    uint8_t numberOfDigits = getNumberOfDigits(number);

    uint8_t xPosition;
    uint8_t digit;
    for(uint8_t i = 0; i < numberOfDigits; i++){
        xPosition = 2 + xOffset + (numberOfDigits - (i + 1)) * 4;
        digit = number / ((int)pow(10, i)) % 10;
        projectDigit(digit, xPosition, yOffset);
    }
    FastLED.show();
}

void projectNumber(uint16_t number, uint8_t xOffset, uint8_t yOffset, uint8_t zeroPad){
    // not even gonna hide my awful code, i swear i will prettify this later on and even might document something
    uint8_t numberOfDigits = max(getNumberOfDigits(number), zeroPad);

    uint8_t xPosition;
    uint8_t digit;
    for(uint8_t i = 0; i < numberOfDigits; i++){
        xPosition = 2 + xOffset + (numberOfDigits - (i + 1)) * 4;
        digit = number / ((int)pow(10, i)) % 10;
        projectDigit(digit, xPosition, yOffset);
    }
    FastLED.show();
}

void projectWord(uint8_t numberOfCharacters, char* string){

}

void projectTime(uint8_t hour, uint8_t minute){
    // This is ugly, but i just want something working
    projectNumber(hour, 1, 1, 2);
    projectNumber(minute, 11, 1, 2);
    projectCharacter(0, 10, 1);
}

void projectCharacter(uint8_t asciiCode, uint8_t xOffset, uint8_t yOffset){
    uint8_t yPosition, xPosition, ledIndex;
    for(uint8_t y = 0; y < 5; y++){
        for(uint8_t x = 0; x < 3; x++){
            if(characters[asciiCode][y][x]){
                yPosition = y + yOffset;
                xPosition = x + xOffset - ((yPosition > wallHeight / 2) ? (yPosition - wallHeight / 2) : 0);
                if(yPosition < wallHeight || xPosition < wallWidth - abs(-(wallHeight / 2) + yPosition)){
                    ledIndex = ledMap[yPosition][xPosition];
                    leds[ledIndex] = CRGB::White;
                }
            }
        }
    }
    FastLED.show();
}

void projectPattern(uint8_t *pattern, uint8_t xOffset, uint8_t yOffset){
    // this is true for all digits
    //projectPattern(pattern, xOffset, yOffset, 3, 4);
}
/**
void projectPattern(uint8_t *pattern, uint8_t xOffset, uint8_t yOffset, uint8_t patternWidth, uint8_t patternHeight){
    uint8_t yPosition, xPosition, ledIndex;
    for(uint8_t y = 0; y < patternHeight; y++){
        for(uint8_t x = 0; x < patternWidth; x++){
            if(pattern[y][x]){
                yPosition = y + yOffset;
                xPosition = x + xOffset - ((yPosition > wallHeight / 2) ? (yPosition - wallHeight / 2) : 0);
                if(yPosition < wallHeight || xPosition < wallWidth - abs(-(wallHeight / 2) + yPosition)){
                    ledIndex = ledMap[yPosition][xPosition];
                    leds[ledIndex] = CRGB::White;
                }
            }
        }
    }
}**/

void clearActiveLeds(){
    for(uint8_t i = 0; i < numberOfLeds; i++){
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}

void backGroundEvent(){

}

void adjustBrightness(uint16_t brightness){

}
