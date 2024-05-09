#ifndef VOICE_HANDLING
#define VOICE_HANDLING

#include <cstdint>

void initVR();
void loadDefaultVR();
void loadOnOff();
void loadOnOffBrighterDarker();
void handleVR();
void addToEventReponse(uint8_t index, void (*response)());


#endif
