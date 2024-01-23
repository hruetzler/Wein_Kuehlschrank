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

    bool getZustand(){
      return zustand["verriegelt"];
    }

    void setZustand(bool zustand){
      this->zustand["verriegelt"] = zustand;
    }

    void checkCode(String code);




  private:
    JsonDocument zustand;
    String zustandString;
    JsonDocument details;
    AsyncWebServer *pServer;
    fs::FS* dateisystem;
    const char* username = "admin";
    const char* password = "admin";

    void SERVER();
    void loadJson();
    void saveJson();



};
