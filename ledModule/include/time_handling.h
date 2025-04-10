#ifndef TIME_HANDLING
#define TIME_HANDLING

#include <cstdint>
#include <time.h>
#include <NTPClient.h>


struct simpleTime {
    uint8_t hour;
    uint8_t minute;
};

typedef struct timerElement timerElement;


bool buildTimeConnection();
void getSimpleTime(struct simpleTime *currentTime);
bool setTimerMilliseconds(uint8_t milliseconds);
void setTimerMillisecondsCallback(uint8_t milliseconds, void (*callbackFunction)());
bool setTimerSeconds(uint8_t seconds);
void setTimerSecondsCallback(uint8_t seconds, void (*callbackFunction)());
bool setTimerMinutes(uint8_t minutes);
void setTimerMinutesCallback(uint8_t minutes, void (*callbackFunction)());
bool checkTimer(timerElement* timerPtr);
timerElement* addTimer(uint8_t seconds);
uint8_t deleteTimer(timerElement* timerPtr);
timerElement* resetTimer(timerElement* timerPtr);

#endif
