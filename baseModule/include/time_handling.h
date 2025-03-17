#ifndef TIME_HANDLING
#define TIME_HANDLING

#include <cstdint>

typedef struct {
    uint8_t hour;
    uint8_t minute;
}simpleTime;

void setupNTPServer();
bool buildTimeConnection();
bool updateTimeClient();
void getSimpleTime(simpleTime *currentTime);
bool setTimerMilliseconds(uint8_t milliseconds);
void setTimerMillisecondsCallback(uint8_t milliseconds, void (*callbackFunction)());
bool setTimerSeconds(uint8_t seconds);
void setTimerSecondsCallback(uint8_t seconds, void (*callbackFunction)());
bool setTimerMinutes(uint8_t minutes);
void setTimerMinutesCallback(uint8_t minutes, void (*callbackFunction)());
uint32_t getEpochTime();

#endif
