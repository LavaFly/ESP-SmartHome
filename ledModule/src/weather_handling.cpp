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

    doc.clear();
}

void printForecastData(){
    char path[120] = "http://api.openweathermap.org/data/2.5/forecast?lat=49.0047&lon=8.3858&units=metric&appid=";
    char completePath[128];
    strcpy(completePath, path);
    strcat(completePath, API);
    JsonDocument filter;
    filter["list"][0]["dt"] = true;
    filter["list"][0]["main"]["temp"] = true;
    filter["list"][0]["weather"][0]["description"] = true;

    DeserializationError err = deserializeJson(doc, httpGetRequestStream(completePath), DeserializationOption::Filter(filter));
    httpEndRequestStream();

    if(err.Ok){
        Serial.println(err.c_str());
        Serial.println(err.code());
        Serial.println("fuck");
        return;
    }

    Serial.println(doc["list"][3]["dt"].as<long>());
    Serial.println(doc["list"][3]["main"]["temp"].as<float>());
    Serial.println(doc["list"][3]["weather"][0]["description"].as<String>());


    doc.clear();
}

void getWeatherDescription(char* weatherDescription){
    char path[120] = "http://api.openweathermap.org/data/2.5/forecast?lat=49.0047&lon=8.3858&units=metric&appid=";
    char completePath[128];
    strcpy(completePath, path);
    strcat(completePath, API);
    JsonDocument filter;
    filter["list"][0]["dt"] = true;
    filter["list"][0]["main"]["temp"] = true;
    filter["list"][0]["weather"][0]["description"] = true;

    DeserializationError err = deserializeJson(doc, httpGetRequestStream(completePath), DeserializationOption::Filter(filter));
    httpEndRequestStream();

    if(err.Ok){
        Serial.println(err.c_str());
        Serial.println(err.code());
        Serial.println("fuck");
        return;
    }

    /**
    Serial.println(doc["list"][3]["dt"].as<long>());
    Serial.println(doc["list"][3]["main"]["temp"].as<float>());
    Serial.println(doc["list"][3]["weather"][0]["description"].as<String>());
    **/

    const char* data = doc["list"][3]["weather"][0]["description"];
    strcpy(weatherDescription, data);

    doc.clear();
}
