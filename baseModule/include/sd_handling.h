#ifndef SD_HANDLING
#define SD_HANDLING

#include <SdFat.h>
#include <SPI.h>

#define SD_SCK_PIN      14 // D5
#define SD_MISO_PIN     12 // D6
#define SD_MOSI_PIN     13 // D7
#define SD_CHIP_SELECT_PIN  15 // D8

void initSDCard();
void dumpSDInfo();
void createDir();
void writeToFile();
void readFromFile();
void readDir();
void removeDir();

void writeJsonToFile(const char* jsonContent);
void readJsonFile();

#endif