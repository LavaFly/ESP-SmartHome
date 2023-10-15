// Handles Connection to LED Strip
#ifndef LED_HANDLING
#define LED_HANDLING

#include <FastLED.h>

#define LED_PIN 14
#define PHOTO_PIN 0
//#define BUTTON_PIN 5

// partly copied from an old project, should hopefully gradually be improved

void led_map_init();
void init_leds();
void buildLEDConnection();
void project_multiple_chars(uint8_t num_of_chars, uint8_t* char_list, uint8_t* x_offset_list, uint8_t y_offset);
void project_number(uint16_t number);
void allLEDSoff();
void backGroundEvent();
void adjustBrightness(uint16_t brightness);

#endif
