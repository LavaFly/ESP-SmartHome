#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <cstring>
#include "ArduinoJson/Json/JsonDeserializer.hpp"
#include "webserver_handling.h"
#include "weather_handling.h"
#include "wmo_handling.h"

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

void getWeatherDescription(char* weatherDescription, uint8_t hourOfTheDay){
    char path[138] = "https://api.open-meteo.com/v1/forecast?latitude=49&longitude=11&hourly=weather_code,temperature_180m&forecast_days=1&timeformat=unixtime";
    //http://api.openweathermap.org/data/3.0/onecall?lat=49,5986&lon=10.9675&exclude=current,minutely,daily,alerts&units=metric&appid=

    JsonDocument filter;
    filter["hourly"]["weather_code"] = true;
    filter["hourly"]["temperature_180m"] = true;

    DeserializationError err = deserializeJson(doc, httpGetRequestStream(path), DeserializationOption::Filter(filter));
    httpEndRequestStream();

    if(err.Ok){
        Serial.println(err.c_str());
        Serial.println(err.code());
        Serial.println("fuck");
        return;
    }

    Serial.println(doc["hourly"]["temperature_180m"][7]);
    Serial.println(doc["hourly"]["weather_code"][7]);
    Serial.println(getDescriptionFromWMO(doc["hourly"]["weather_code"][7]));


    /**
    Serial.println(doc["list"][3]["dt"].as<long>());
    Serial.println(doc["list"][3]["main"]["temp"].as<float>());
    Serial.println(doc["list"][3]["weather"][0]["description"].as<String>());
    **/

    // i need to filter for the specified hour of the day
    //  get current time from ntp server
    //  calculate current hour of day
    //  if currHour > speciHour, then weatherDescription = "time already passed"
    //  else
    //   speciHour timestamp = (speciHour - currHour) * 3600 + currentTime
    //   iterate through list and find closest match
    //
    // yeah this is completly wrong, the response it a forecast for the whole day, even though
    // this forecast may contain past events, the index is just the hour of the day

    /**
    const char* data = doc["list"][3]["weather"][0]["description"];
    //data = "rain"; mock
    float tempe = doc["list"][3]["main"]["temp"].as<int>();
    // tempe = 23; mock
    char tempeBuf[5]; // Buffer big enough for 7-character float
    //dtostrf(tempe, 4, 2, tempeBuf);
    itoa(tempe, tempeBuf, 10);
    Serial.print("temp = ");
    Serial.println(tempeBuf);
    const char* postFix = "*C";
    strcpy(weatherDescription, data);
    strcat(weatherDescription, " ");
    strcat(weatherDescription, tempeBuf);
    strcat(weatherDescription, postFix);
    **/

    doc.clear();
}
