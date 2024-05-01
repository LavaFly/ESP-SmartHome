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
bool setTimerSeconds(uint8_t seconds);


#endif
