#include "led_handling.h"
#include "characters.h"
#include "alphabet.h"
#include "alphabet_lower.h"


#define MAX_BRIGHTNESS 240
#define MIN_BRIGHTNESS 1


const int brightnessInPin = A0;

// dimension of the led-wall
const uint8_t wallHeight = 7;
const uint8_t wallWidth = 23;
const uint8_t numberOfLeds = 149;

CRGB leds[numberOfLeds];

uint8_t activeLeds[numberOfLeds];

// iliffe vector
uint8_t* ledMap[7];

uint8_t *charList = 0;
int8_t *offsetList = 0;
int8_t widthOfProjectedString = 0;
uint8_t lengthOfProjectedString = 0;
uint8_t animationDuration = 0;
//extern uint8_t numbers[10][5][3];

CHSV color(0, 255, 180);
CHSV returnColor(0, 255, 180);
uint8_t colorCounter = 0;
CHSV getRainbowColor();
void incColor();

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
    FastLED.setBrightness(MAX_BRIGHTNESS);
}

void projectDigit(uint8_t digit, uint8_t xOffset, uint8_t yOffset){
    uint8_t yPosition, xPosition, ledIndex;
    for(uint8_t y = 1; y < 6; y++){
        for(uint8_t x = 0; x < 3; x++){
            if(characters[digit][y][x]){
                yPosition = y + yOffset;
                xPosition = x + xOffset - ((yPosition > wallHeight / 2) ? (yPosition - wallHeight / 2) : 0);
                if((yPosition < wallHeight && xPosition < wallWidth - abs(-(wallHeight / 2) + yPosition) ) && (xPosition * yPosition >= 0)){
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
    adjustBrightness();
    // This is ugly, but i just want something working
    projectNumber(hour, 1, 0, 2);
    projectNumber(minute, 11, 0, 2);
    projectCharacter(63, 10, 0);
}

void mothersDayMessage(){
    // print "*heart* Alles Gute zum Muttertag *heart*"
    // 66 = heart symbol
    const char* inputString = "* Alles Gute zum Muttertag *";
    lengthOfProjectedString = strlen(inputString);
    charList = (uint8_t*)malloc(sizeof(uint8_t) * lengthOfProjectedString);
    offsetList = (int8_t*)malloc(sizeof(uint8_t) * lengthOfProjectedString);

    generateCharList(inputString, lengthOfProjectedString, charList);
    // modify charList to include heart symbol
    charList[0] = 66;
    charList[lengthOfProjectedString - 1] = 66;

    //generateOffsetList((char*)charList, lengthOfProjectedString, 25, offsetList);
    generateOffsetList(charList, lengthOfProjectedString, 25, offsetList);
    widthOfProjectedString = getLengthOfString(charList, offsetList, lengthOfProjectedString);
    animationDuration = widthOfProjectedString + offsetList[0];

    // project once, afterwards use advanceSlideAnimation()
    projectString(charList, lengthOfProjectedString, offsetList);
}

void projectCharacter(uint8_t asciiCode, uint8_t xOffset, uint8_t yOffset){
    adjustBrightness();
    uint8_t yPosition, xPosition, ledIndex;
    for(uint8_t y = 0; y < 5; y++){
        for(uint8_t x = 0; x < 9; x++){
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


// function takes in string, iterates over the complete length of the string
// and converts each char to the corresponding index in the characters-array
// or to empty space if unable to do so
void generateCharList(const char* initialString, uint8_t numberOfCharacters, uint8_t* charList){
    char currentChar = ' ';
    uint8_t lowercaseOffset = 10;
    uint8_t uppercaseOffset = lowercaseOffset + 26;
    for(uint8_t i = 0; i < numberOfCharacters; i++){
        currentChar = *(initialString + i);
        // if currentChar is a digit
        if(currentChar >= '0' && currentChar <= '9'){
            charList[i] = currentChar - '0';

        // if currentChar is a lowercase letter
        } else if(currentChar >= 'a' && currentChar <= 'z'){
            charList[i] = currentChar - 'a' + lowercaseOffset;

        // if currentChar is a uppercase letter
        } else if(currentChar >= 'A' && currentChar <= 'Z'){
            charList[i] = currentChar - 'A' + uppercaseOffset;
        } else if(currentChar == '*'){ // this is a workaround, because ° doesnt fit inside a char
            charList[i] = 65;

        // space if other symbol
        } else {
            charList[i] = 62;
        }
    }
}

// function takes in list of characters as indexes of array and calculates
// the proper offset between the letters
void generateOffsetList(uint8_t* charList, uint8_t numberOfCharacters, int8_t initialOffset, int8_t* offsetList){

    // first offset is initialOffset
    offsetList[0] = initialOffset;
    // for next char
    //  loop through each row in the 2d array of the previous character
    for(uint8_t i = 1; i < numberOfCharacters; i++){
        uint8_t maxPositionOfOne = 0;
        // loop through each row
        for(uint8_t y = 0; y < 6; y++){
            //  store max index of 1 in all rows
            for(uint8_t x = 0; x < 9; x++){
                if(characters[charList[i - 1]][y][x] && maxPositionOfOne < x){
                    maxPositionOfOne = x;
                }
            }
        }

        //  offset is offset of previous character + max index + 1
        offsetList[i] = offsetList[i - 1] + maxPositionOfOne + 2;
        //  (if previous character is a space the + 2)
        if(charList[i] == 62){
            offsetList[i] += 1;
        }
    }
}

int8_t getLengthOfString(uint8_t* charList, int8_t* offsetList, uint8_t numberOfCharacters){
    uint8_t maxPositionOfOne = 0;
    // loop through each row
    for(uint8_t y = 0; y < 6; y++){
        //  store max index of 1 in all rows
        for(uint8_t x = 0; x < 9; x++){
            if(characters[charList[numberOfCharacters - 1]][y][x] && maxPositionOfOne < x){
                maxPositionOfOne = x;
            }
        }
    }
    return maxPositionOfOne + offsetList[numberOfCharacters - 1];
}

void startSlideAnimation(const char* inputString, uint8_t numberOfCharacters){
    lengthOfProjectedString = numberOfCharacters;
    charList = (uint8_t*)malloc(sizeof(uint8_t) * lengthOfProjectedString);
    offsetList = (int8_t*)malloc(sizeof(uint8_t) * lengthOfProjectedString);

    generateCharList(inputString, lengthOfProjectedString, charList);
    generateOffsetList(charList, lengthOfProjectedString, 25, offsetList);
    widthOfProjectedString = getLengthOfString(charList, offsetList, lengthOfProjectedString);
    animationDuration = widthOfProjectedString + offsetList[0];

    // project once, afterwards use advanceSlideAnimation()
    projectString(charList, lengthOfProjectedString, offsetList);
}

bool advanceSlideAnimation(){
    if(animationDuration > 0){
        shiftOffsetToLeft(offsetList, lengthOfProjectedString);
        projectString(charList, lengthOfProjectedString, offsetList);

        animationDuration--;
        //Serial.print("dur: ");
        //Serial.println(animationDuration);
        return true;
    }

    animationDuration = 0;

    if(charList || offsetList){
        Serial.println("Cleanup");
        free(charList);
        free(offsetList);
        charList = 0;
        offsetList = 0;
        clearActiveLeds();
        FastLED.show();
    }

    return false;
}



void shiftOffsetToLeft(int8_t* offsetList, uint8_t numberOfElements){
    for(uint8_t i = 0; i < numberOfElements; i++){
        offsetList[i] -= 1;
    }
}

void shiftOffsetToRight(int8_t* offsetList, uint8_t numberOfElements){
    for(uint8_t i = 0; i < numberOfElements; i++){
        offsetList[i] += 1;
    }
}

void projectString(uint8_t* charList, uint8_t numberOfCharacters, int8_t* offsetList){
    clearActiveLeds();
    adjustBrightness();
    uint8_t yPosition, xPosition, ledIndex;
    //backgroundEvent();
    for(uint8_t z = 0; z < numberOfCharacters; z++){
        for(uint8_t y = 0; y < 6; y++){
            for(uint8_t x = 0; x < 9; x++){
                uint8_t currentPosition = characters[charList[z]][y][x];
                if(currentPosition){
                    yPosition = y + 1;
                    xPosition = x + offsetList[z] - ((yPosition > wallHeight / 2) ? (yPosition - wallHeight / 2) : 0);
                    if((yPosition < wallHeight && xPosition < wallWidth - abs(-(wallHeight / 2) + yPosition) ) && (xPosition * yPosition >= 0)){
                        ledIndex = ledMap[yPosition][xPosition];
                        // TODO check for color spec
                        leds[ledIndex] = CRGB::White;
                        //leds[ledIndex] = getRainbowColor();

                        if(currentPosition == 2){
                            leds[ledIndex] = CRGB::HotPink;
                        }
                    }
                }
            }
        }
    }
    //incColor();
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

CHSV getRainbowColor(){
    returnColor.hue = ( color.hue + 3 * colorCounter ) % 256;
    colorCounter++;
    return returnColor;
}

void incColor(){
    color.hue = ( color.hue + 10 ) % 256;
    colorCounter = 0;
}

void backgroundEvent(){
    CHSV color(0, 255, 180);
        if(colorCounter < 10) {
            colorCounter++;
        } else {
            color.hue = (color.hue + 3) % 256;
            colorCounter = 0;
        }
        for(int i = 0; i < numberOfLeds; i++){
            color.hue = (color.hue + 1) % 256;
            leds[i] = color;
        }
        color.hue = (color.hue + 128) % 256;
        FastLED.show();
}

void adjustBrightness(){
    int mappedValue = map(analogRead(brightnessInPin), 0, 1023, 10, 255);
    FastLED.setBrightness(constrain(mappedValue + 20, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
    Serial.print("measured: ");
    Serial.println(mappedValue);
}

