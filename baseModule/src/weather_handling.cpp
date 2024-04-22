#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "ArduinoJson/Json/JsonDeserializer.hpp"
#include "webserver_handling.h"
#include "weather_handling.h"

JsonDocument doc;
void printWeatherData(){
    // for now, location is fixed, later changeable through WebUI?
    // API is defined in internet_settings which is included in webserver_handling
    char path[82] = "http://api.openweathermap.org/data/2.5/weather?q=Karlsruhe,DE&units=metric&APPID=";
    char completePath[128];
    strcpy(completePath, path);
    strcat(completePath, API);
    const String* content = httpGetRequest(completePath);
    DeserializationError err = deserializeJson(doc, *(content));

    if(err.Ok){
        Serial.println(err.c_str());
        Serial.println(err.code());
        Serial.println("fuck");
        return;
    }

    float tmp = doc["main"]["temp"];
    float humid = doc["main"]["humidity"];
    Serial.print("temperature: ");
    Serial.println(tmp);
    Serial.print("humitity: ");
    Serial.println(humid);
}
