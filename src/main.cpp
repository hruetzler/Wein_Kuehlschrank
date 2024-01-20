#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

const char* ssid = "WLAN Name";
const char* password = "Passwort";

JsonDocument zustand;
JsonDocument details;


AsyncWebServer server(80);


void loadJson();
void saveJson();
void SERVER();

void setup() {
  Serial.begin(115200);
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  // LittleFS.beginn();
  
  WiFi.begin(ssid, password);
  Serial.print("Connection to Wifi . . .");
  while (WiFi.status() != WL_CONNECTED )
  {
    delay(500);
    Serial.print(" .");
  }
  Serial.println();
  Serial.println(WiFi.localIP());

  loadJson();

  SERVER();

  }

void loop() {
  // put your main code here, to run repeatedly:
}








void SERVER()
{

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Seite aufgerufen");
    request->send(LittleFS, "/index.html", String(), false);
  });

   server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/script.js", "text/javascript");
  });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/favicon.ico", "text/plain");
  });

    server.on("/addIcon.png", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/addIcon.png", "text/png");
  });


  server.on("/statusSwitch", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", zustand.as<String>());
  });

  
  server.onRequestBody([](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total){
    if (request->url() == "/swClick" && request->method() == HTTP_PUT){

      request->send(200, "text/plain", "Alle geklappt");
      Serial.println((const char*)data);
      Serial.println(len);
      if (*data == '1'){
        Serial.println(":)");
        zustand["verriegelt"] = true;
      }else{
        Serial.println(":(");
        zustand["verriegelt"] = false;
      }
    }
    if (request->url() == "/nueDaten" && request->method() == HTTP_PUT){
      request->send(200, "text/plain", "Alle geklappt");
      
      JsonDocument doc;
      deserializeJson(doc, data);
      String name = doc["name"];
      String datum = doc["datum"];
      String uhrzeit = doc["uhrzeit"];
      String stunden = doc["stunden"];
      String code = doc["code"];
      Serial.println(name);
      Serial.println(datum);
      Serial.println(uhrzeit);
      Serial.println(stunden);
      Serial.println(code);

      details["Name"] = name;
      details["Datum"] = datum;
      details["Uhrzeit"] = uhrzeit;
      details["Dauer"] = stunden;
      details["Code"] = code;

      JsonArray zeiten = zustand["zeiten"];
      

      zeiten.add(details);

      saveJson();

    }
    if (request->url() == "/datenAktualisieren" && request->method() == HTTP_PATCH){
      request->send(200, "text/plain", "Alle geklappt");
      JsonDocument doc;
      deserializeJson(doc, data);

      int id = doc["id"];
      String name = doc["name"];
      String datum = doc["datum"];
      String uhrzeit = doc["uhrzeit"];
      String stunden = doc["stunden"];
      String code = doc["code"];

      details["Name"] = name;
      details["Datum"] = datum;
      details["Uhrzeit"] = uhrzeit;
      details["Dauer"] = stunden;
      details["Code"] = code;

      zustand["zeiten"][id] = details;

      saveJson();
    }
    if (request->url() == "/datenDelete" && request->method() == HTTP_DELETE){
      request->send(200, "text/plain", "Alle geklappt");

      JsonDocument doc;
      deserializeJson(doc, data);
      int id = doc["id"];

      JsonArray zeiten = zustand["zeiten"];
      Serial.println(id);
      zeiten.remove(id);


      saveJson();
      
    
    }

  });
  server.onNotFound([](AsyncWebServerRequest *request){
    if (request->url() != "/swClick"){
      request->send(404, "text/plain", "Invalid request");
    }

  });

  server.begin();

}

void loadJson(){
  File file = LittleFS.open("/json/daten.json", "r");
  DeserializationError error = deserializeJson(zustand, file);

  if(error){
    Serial.println("Es ist ein Fehler beim lesen aufgetreten");
     zustand["verriegelt"] = false;

    details["Name"] = "Erik";
    details["Datum"] = "2007-12-24";
    details["Uhrzeit"] = "12:00";
    details["Dauer"] = "3";
    details["Code"] = "5763";

    JsonArray zeiten = zustand["zeiten"].to<JsonArray>();
    zeiten.add(details);

    details["Name"] = "Henri";
    details["Datum"] = "2005-10-22";
    details["Uhrzeit"] = "12:00";
    details["Dauer"] = "3";
    details["Code"] = "5763";

    zeiten.add(details);
  }

  file.close();
}

void saveJson(){
  File file = LittleFS.open("/json/daten.json", "w");
  serializeJson(zustand, file);
  file.close();
}