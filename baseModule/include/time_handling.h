#ifndef TIME_HANDLING
#define TIME_HANDLING

#include <cstdint>

typedef struct {
    uint8_t hour;
    uint8_t minute;
}simpleTime;

bool buildTimeConnection();
bool updateTimeClient();
void getSimpleTime(simpleTime *currentTime);
bool setTimerMilliseconds(uint8_t milliseconds);
bool setTimerSeconds(uint8_t seconds);
void setTimerSecondsCallback(uint8_t seconds, void (*callbackFunction)());
uint32_t getEpochTime();

#endif
