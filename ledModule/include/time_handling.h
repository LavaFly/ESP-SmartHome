#ifndef TIME_HANDLING
#define TIME_HANDLING

#include <cstdint>
#include <time.h>
#include <NTPClient.h>


struct simpleTime {
    uint8_t hour;
    uint8_t minute;
};



bool buildTimeConnection();
void getSimpleTime(struct simpleTime *currentTime);
bool setTimerMilliseconds(uint8_t milliseconds);
void setTimerMillisecondsCallback(uint8_t milliseconds, void (*callbackFunction)());
bool setTimerSeconds(uint8_t seconds);
void setTimerSecondsCallback(uint8_t seconds, void (*callbackFunction)());
bool setTimerMinutes(uint8_t minutes);
void setTimerMinutesCallback(uint8_t minutes, void (*callbackFunction)());
bool checkTimer(uint8_t timerIndex);
uint8_t addTimer(uint8_t seconds);
uint8_t deleteTimer(uint8_t timerIndex);
uint8_t resetTimer(uint8_t timerIndex);



#endif
