// Handles Connection to LED Strip
#ifndef LED_HANDLING
#define LED_HANDLING

#include <FastLED.h>

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

void projectExampleString(int slideOffset);

void projectTest();
void buildLedConnection();
void clearActiveLeds();
void backgroundEvent();
void adjustBrightness(uint16_t brighness);

#endif
