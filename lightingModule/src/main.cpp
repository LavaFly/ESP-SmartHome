#include "ir_handling.h"
#include "webserver_handling.h"

void setup(){
   Serial.begin(9600);
   buildRouterConnection();
   buildIrConnection();
   initWebserver();
   setupMDNS();
}

void loop(){
    MDNS.update();
    loopOTA();
    if(checkForNewMessage() != 0){
        sendSignal();
    }
}
