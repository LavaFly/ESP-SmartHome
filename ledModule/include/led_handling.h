// Handles Connection to LED Strip
#ifndef LED_HANDLING
#define LED_HANDLING

#include <FastLED.h>
#include <cstdint>

#define LEDPIN 12
#define PHOTO_PIN 0
//#define BUTTON_PIN 5

struct activeLed {
   uint8_t value;
   uint8_t xPosition;
   uint8_t yPosition;
};

void initialiseLedMap();
void initialiseLedMapBorders();
uint8_t getNumberOfDigits(uint16_t number);
//void projectPattern(uint8_t *pattern, uint8_t xOffset, uint8_t yOffset);
void projectPattern(uint8_t *pattern, uint8_t xOffset, uint8_t yOffset, uint8_t patternWidth, uint8_t patternHeight);
void projectDigit(uint8_t digit, uint8_t xOffset, uint8_t yOffset);
void projectNumber(uint16_t number);
void projectNumber(uint16_t number, uint8_t xOffset, uint8_t yOffset);
void projectNumber(uint16_t number, uint8_t xOffset, uint8_t yOffset, uint8_t zeroPad);
void projectCharacter(uint8_t asciiCode, uint8_t xOffset, uint8_t yOffset);
void projectWord(uint16_t numberOfCharacters, char* string);
void projectTime(uint8_t hour, uint8_t minute);

void mothersDayMessage();
// function takes in string, iterates over the complete length of the string
// and converts each char to the corresponding index in the characters-array
// or to empty space if unable to do so
void generateCharList(const char* initialString, uint8_t numberOfCharacters, uint8_t* charList);

// function takes in list of characters as indexes of array and calculates
// the proper offset between the letters
void generateOffsetList(char* charList, uint8_t numberOfCharacters, int8_t initialOffset, int8_t* offsetList);
void generateOffsetList(uint8_t* charList, uint8_t numberOfCharacters, int8_t initialOffset, int8_t* offsetList);

void projectString(uint8_t* charList, uint8_t numberOfCharacters, int8_t* offsetList);

int8_t getLengthOfString(uint8_t* charList, int8_t* offsetList, uint8_t numberOfCharacters);

void shiftOffsetToLeft(int8_t* offsetList, uint8_t numberOfElements);
void shiftOffsetToRight(int8_t* offsetList, uint8_t numberOfElements);

void startSlideAnimation(const char* inputString, uint8_t numberOfCharacters);
bool advanceSlideAnimation();

void projectStringCentric(uint8_t* charList, uint8_t numberOfCharacters, int8_t offsetList);


void projectTest();
void buildLedConnection();
void clearActiveLeds();
void backgroundEvent();
void adjustBrightness();

#endif
