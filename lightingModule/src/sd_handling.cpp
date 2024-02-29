#include "sd_handling.h"


// https://randomnerdtutorials.com/esp32-web-server-microsd-card/
void initSDCard(){
    if(!sd.begin(SD_CHIP_SELECT_PIN, SD_SCK_MHZ(50))){
        sd.initErrorHalt(&Serial);
        return;
    }
    Serial.println("SDCard Mount successful");
}
