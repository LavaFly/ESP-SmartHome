#ifndef WMO_HANDLING
#define WMO_HANDLING
// Weather code descriptions stored in PROGMEM (flash)
const char desc_0[] PROGMEM = "clear sky";
const char desc_1[] PROGMEM = "mainly clear";
const char desc_2[] PROGMEM = "partly cloudy";
const char desc_3[] PROGMEM = "overcast";
const char desc_45[] PROGMEM = "fog";
const char desc_48[] PROGMEM = "depositing rime fog";
const char desc_51[] PROGMEM = "light drizzle";
const char desc_53[] PROGMEM = "moderate drizzle";
const char desc_55[] PROGMEM = "dense drizzle";
const char desc_56[] PROGMEM = "light freezing drizzle";
const char desc_57[] PROGMEM = "dense freezing drizzle";
const char desc_61[] PROGMEM = "slight rain";
const char desc_63[] PROGMEM = "moderate rain";
const char desc_65[] PROGMEM = "heavy rain";
const char desc_66[] PROGMEM = "light freezing rain";
const char desc_67[] PROGMEM = "heavy freezing rain";
const char desc_71[] PROGMEM = "slight snowfall";
const char desc_73[] PROGMEM = "moderate snowfall";
const char desc_75[] PROGMEM = "heavy snowfall";
const char desc_77[] PROGMEM = "snow grains";
const char desc_80[] PROGMEM = "slight rain showers";
const char desc_81[] PROGMEM = "moderate rain showers";
const char desc_82[] PROGMEM = "violent rain showers";
const char desc_85[] PROGMEM = "slight snow showers";
const char desc_86[] PROGMEM = "heavy snow showers";
const char desc_95[] PROGMEM = "thunderstorm";
const char desc_96[] PROGMEM = "thunderstorm with slight hail";
const char desc_99[] PROGMEM = "thunderstorm with heavy hail";
const char desc_unknown[] PROGMEM = "unknown";

// Struct to map code to string in flash
struct WeatherCode {
  uint8_t code;
  const char* desc;
};

// Weather code table
const WeatherCode weatherCodes[] PROGMEM = {
  {0, desc_0}, {1, desc_1}, {2, desc_2}, {3, desc_3},
  {45, desc_45}, {48, desc_48},
  {51, desc_51}, {53, desc_53}, {55, desc_55},
  {56, desc_56}, {57, desc_57},
  {61, desc_61}, {63, desc_63}, {65, desc_65},
  {66, desc_66}, {67, desc_67},
  {71, desc_71}, {73, desc_73}, {75, desc_75},
  {77, desc_77},
  {80, desc_80}, {81, desc_81}, {82, desc_82},
  {85, desc_85}, {86, desc_86},
  {95, desc_95}, {96, desc_96}, {99, desc_99}
};

// Lookup function
void getDescriptionFromWMO(uint8_t code, char* buffer, size_t bufferSize);

#endif
