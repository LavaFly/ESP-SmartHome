#include "led_handling.h"
#include "characters.h"

// dimension of the led-wall
const uint8_t wallHeight = 7;
const uint8_t wallWidth = 23;
const uint8_t numberOfLeds = 149;

CRGB leds[numberOfLeds];

uint8_t activeLeds[numberOfLeds];

uint8_t* ledMap[7];


// width = ((num - (height + 1)/2)^2) / height + (height + 1/2)


void initialiseLedMap(){
    // i will clean this mess up another time, it works for now
    // docu will also follow soon
    uint8_t segmentWidth = 0;
    for(uint8_t i = 0; i < wallHeight; i++){
        segmentWidth = wallWidth - abs(-(wallHeight / 2) + i);
        *(ledMap + i) = new uint8_t[segmentWidth];
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

uint8_t getNumOfDigits(uint16_t number){
    if(number == 0) return 1;
    return floor(log10(abs(number))) + 1;
}

void updateLedWall(){

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
}

void projectNumber(uint16_t number){

}

void projectWord(uint8_t numberOfCharacters, char* string){

}

void projectTime(uint8_t hour, uint8_t minute){
    // This is ugly, but i just want something working

}

void project_number(uint16_t number){

}

void project_multiple_chars(uint8_t num_of_chars, uint8_t* char_list, uint8_t* x_offset_list, uint8_t y_offset){
    uint8_t counter, x_pos, y_pos, ledIndex;
    //for(uint8_t z = 0; z <
}

void allLEDSoff(){

}

void clearActiveLeds(){

}

void backGroundEvent(){

}

void adjustBrightness(uint16_t brightness){

}
