#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>



class WeinWebserver {

  public:
    WeinWebserver(fs::FS* fs) : dateisystem(fs){
      pServer = new AsyncWebServer(80);
      
      loadJson();
      SERVER();


    }

    ~WeinWebserver(){
      delete pServer;
    }

    void test(){
      Serial.println(zustandString);
    }




  private:
    JsonDocument zustand;
    String zustandString;
    JsonDocument details;
    AsyncWebServer *pServer;
    fs::FS* dateisystem;

    void SERVER();
    void loadJson();
    void saveJson();

};
