#ifndef VOICE_HANDLING
#define VOICE_HANDLING

#include <cstdint>

void initVR();
void loadDefaultResponse();
void loadOnOff();
void loadOnOffBrighterDarker();
void handleVR();
void addToEventReponse(uint8_t index, uint8_t (*response)());
void setupEventResponse();
bool checkVrTimer(uint8_t vrResetTimeSeconds);
void clearVrTimer();



uint8_t vr_pc();
uint8_t vr_light();
uint8_t vr_time();
uint8_t vr_weather();
uint8_t vr_temperature();
uint8_t vr_co2();
uint8_t vr_on();
uint8_t vr_off();
uint8_t vr_brighter();
uint8_t vr_darker();
uint8_t vr_empty();


#endif
