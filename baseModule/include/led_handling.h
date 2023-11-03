// Handles Connection to LED Strip
#ifndef LED_HANDLING
#define LED_HANDLING

#include <FastLED.h>

#define LEDPIN 14
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
void projectDigit(uint8_t digit, uint8_t xOffset, uint8_t yOffset);
void projectNumber(uint16_t number);
void projectNumber(uint16_t number, uint8_t xOffset, uint8_t yOffset);
void projectWord(uint16_t numberOfCharacters, char* string);
void projectTime(uint8_t hour, uint8_t minute);

void projectTest();
void buildLedConnection();
void clearActiveLeds();
void backgroundEvent();
void adjustBrightness(uint16_t brighness);

#endif
