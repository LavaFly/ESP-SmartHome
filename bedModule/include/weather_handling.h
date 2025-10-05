#ifndef WEATHER_HANDLING
#define WEATHER_HANDLING

extern const char* API;

void printWeatherData();

void printForecastData();

void getWeatherDescription(char* weatherDescription, uint8_t hourOfTheDay);

#endif
