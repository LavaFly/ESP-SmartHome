#include "wmo_handling.h"

void getDescriptionFromWMO(uint8_t code, char* buffer, size_t bufferSize) {
  const size_t numCodes = sizeof(weatherCodes) / sizeof(weatherCodes[0]);

  for (size_t i = 0; i < numCodes; i++) {
    WeatherCode entry;
    memcpy_P(&entry, &weatherCodes[i], sizeof(WeatherCode));
    if (entry.code == code) {
      strncpy_P(buffer, (PGM_P)entry.desc, bufferSize - 1);
      buffer[bufferSize - 1] = '\0'; // Ensure null-termination
      return;
    }
  }

  // Fallback if code not found
  strncpy_P(buffer, desc_unknown, bufferSize - 1);
  buffer[bufferSize - 1] = '\0';
}
