#include "sd_handling.h"
#include "sensor_handling.h"

SdFat sd;
SdFile myFile;
SdFile myDir;

uint32_t cardSize;
uint32_t eraseSize;

void readDir(){
    Serial.println("readDir");
    if(myDir.open("/testFolder")){
        while(myFile.openNext(&myDir, O_RDONLY));
            myFile.printFileSize(&Serial);
            Serial.write("  ");
            myFile.printModifyDateTime(&Serial);
            Serial.write("  ");
            myFile.printName(&Serial);

            if(myFile.isDir())
                Serial.write('/');
            Serial.println("");

            myFile.close();
    } else {
        Serial.println("opening dir failed!");
    }
}
void removeDir(){
    Serial.println("removeDir");
    if(sd.rmdir("testFolder")){
        Serial.println("removing worked");
    } else {
        Serial.println("removing failed");
    }
}
void createDir(){
    Serial.println("createDir");
    if(!sd.exists("testFolder")){
        if(myDir.open("/")){
            if(sd.mkdir("testFolder")){
                Serial.println("testfolder created");
            } else {
                Serial.println("testfolder not created");
            }
        } else {
            Serial.println("open testfolder failed");
        }
    } else {
        Serial.println("folder already exists");
    }
}
void writeToFile(){
    Serial.println("writeToFile");
    //myFile = sd.open("testFolder/text.txt", FILE_WRITE);
    myFile.open("testFolder/text.txt", FILE_WRITE);

    if(myFile){
        Serial.println("writing to file");
        myFile.println("test123 123");
        myFile.close();
    } else {
        Serial.println("error opening");
    }
}

void writeJsonToFile(const char* jsonContent){
    Serial.println("starting writing json to file");

    myFile.open("testFolder/sensor.json", FILE_WRITE);

    if(myFile){
        myFile.println(jsonContent);
        myFile.close();
        Serial.println("successful in writing to file");
    } else {
        Serial.println("error in opening file");
    }
}

// function for preparing for respone
//  check if another file is open, close if true
//  open desired file
bool prepareResponse(const char* pathToFile){
    myFile.open(pathToFile, FILE_READ);
    if(myFile){
        return true;
    } else {
        return false;
    }
}

size_t readFileForResponse(uint8_t* buffer, size_t maxLen, size_t index) {
    if (myFile.available()) {
        return myFile.read(buffer, maxLen);
    } else {
        // close file
        myFile.close();
        return 0;
    }
}

void readFromFile(){
    Serial.println("readFromFile");
    //myFile = sd.open("testFolder/text.txt");
    myFile.open("testFolder/text.txt", FILE_READ);

    if(myFile){
        while(myFile.available()){
            Serial.write(myFile.read());
        }
        myFile.close();
    } else {
        Serial.println("failed to read");
    }
}

void readJsonFile(){
    Serial.println("readFromFile");
    //myFile = sd.open("testFolder/text.txt");
    myFile.open("testFolder/sensor.json", FILE_READ);

    if(myFile){
        while(myFile.available()){
            Serial.write(myFile.read());
        }
        myFile.close();
    } else {
        Serial.println("failed to read");
    }
    myFile.println();
}

void initSDCard(){
    uint32_t t = millis();

    if(!sd.begin(SD_CHIP_SELECT_PIN, SD_SCK_MHZ(50))) {
        Serial.println("init failed");
        return;
    }

    t = millis() - t;
    cardSize = sd.card()->cardSize();

    if(cardSize == 0){
        Serial.println("cardSize failed");
        return;
    }
    Serial.println("init done");
}

void dumpSDInfo(){
    Serial.println("dumpSDInfo");
    Serial.println(SD_FAT_VERSION);
}

